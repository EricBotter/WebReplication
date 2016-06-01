using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WebReplicationDemo
{
    public partial class MainForm : Form
    {
        NetworkEntity resolver, proxy;
        List<NetworkEntity> fileservers = new List<NetworkEntity>();
        Dictionary<int, NetworkEntity> proxySocketMap = new Dictionary<int, NetworkEntity>();
        Dictionary<int, EntityConnection> proxyConnections = new Dictionary<int, EntityConnection>();

        private NetworkEntity _activeServer;
        NetworkEntity activeServer
        {
            get
            {
                return _activeServer;
            }
            set
            {
                if (_activeServer != null)
                {
                    _activeServer.pictureBox.BackColor = Color.Transparent;
                }
                _activeServer = value;

                if (_activeServer == null)
                {
                    removeServerButton.Enabled = killServerButton.Enabled = false;
                }
                else
                {
                    _activeServer.pictureBox.BackColor = Color.LightBlue;
                    if (_activeServer == proxy || _activeServer == resolver)
                        removeServerButton.Enabled = killServerButton.Enabled = false;
                    else
                        killServerButton.Enabled = !(removeServerButton.Enabled = _activeServer.hasExited());

                    connectionsTreeView.SuspendLayout();
                    connectionsTreeView.Nodes.Clear();
                    if (_activeServer == proxy)
                    {
                        connectionsTreeView.Nodes.AddRange(
                        (
                            from item in proxyConnections
                            select new TreeNode("Socket " + item.Key + " conn. on port " + item.Value.server.port,
                            (
                                from x in item.Value.packets
                                where x.type == Packet.PacketType.COMPLETED
                                group x by x.param1.Substring(0, x.param1.IndexOf('/')) into y
                                select new TreeNode(y.Key,
                                (
                                    from z in y
                                    select new TreeNode(z.param2 + " -- " + z.param1.Substring(z.param1.IndexOf('/')),
                                            z.param2 == "200" ? 0 : 1, z.param2 == "200" ? 0 : 1)
                                ).ToArray())
                            ).ToArray())
                        ).ToArray());
                    }
                    else if (_activeServer != resolver)
                    {
                        connectionsTreeView.Nodes.AddRange(
                        (
                            from item in proxyConnections
                            where item.Value.server == activeServer
                            select new TreeNode("Connection with proxy",
                            (
                                from x in item.Value.packets
                                where x.type == Packet.PacketType.COMPLETED
                                group x by x.param1.Substring(0, x.param1.IndexOf('/')) into y
                                select new TreeNode(y.Key,
                                (
                                    from z in y
                                    select new TreeNode(z.param2 + " -- " + z.param1.Substring(z.param1.IndexOf('/')),
                                            z.param2 == "200" ? 0 : 1, z.param2 == "200" ? 0 : 1)
                                ).ToArray())
                            ).ToArray())
                        ).ToArray());
                    }
                    connectionsTreeView.ExpandAll();
                    if (connectionsTreeView.Nodes.Count > 0)
                        connectionsTreeView.Nodes[0].EnsureVisible();
                    connectionsTreeView.ResumeLayout();
                }
            }
        }

        int proxyLogLineParsed = 0;

        #region Automatic image positions
        private Size serverImageSize
        {
            get
            {
                int size = Math.Min(serverDisplayPanel.Width, serverDisplayPanel.Height) / 4;
                return new Size(size, size);
            }
        }
        private Point resolverImageLocation
        {
            get
            {
                return new Point((serverDisplayPanel.Width) / 8, (serverDisplayPanel.Height * 5) / 8);
            }
        }
        private Point proxyImageLocation
        {
            get
            {
                return new Point((serverDisplayPanel.Width * 7) / 8 - serverImageSize.Width, (serverDisplayPanel.Height * 5) / 8);
            }
        }
        private int serverImageY
        {
            get
            {
                return serverDisplayPanel.Height / 8;
            }
        }
        #endregion

        public MainForm()
        {
            InitializeComponent();

            resolver = new NetworkEntity("Resolver");
            proxy = new NetworkEntity("Proxy");

            proxy.pictureBox.Click += proxyImage_Click;
            resolver.pictureBox.Click += resolverImage_Click;

            serverDisplayPanel.Controls.Add(resolver.pictureBox);
            serverDisplayPanel.Controls.Add(resolver.label);
            serverDisplayPanel.Controls.Add(proxy.pictureBox);
            serverDisplayPanel.Controls.Add(proxy.label);
        }

        void proxyImage_Click(object sender, EventArgs e)
        {
            activeServer = proxy;
        }
        void resolverImage_Click(object sender, EventArgs e)
        {
            activeServer = resolver;
        }
        void serverImage_Click(object sender, EventArgs e)
        {
            activeServer = fileservers.AsEnumerable().First(x => x.pictureBox == (PictureBox)sender);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            resolver.Start();
            proxy.Start();

            serverDisplayPanel_SizeChanged(sender, e);
            logUpdateTimer.Start();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (addServerButton.Enabled)
            {
                if (MessageBox.Show("Are you sure you want to exit?\nThe child processes will be terminated.", "Confirm", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.No)
                {
                    e.Cancel = true;
                    return;
                }

                killChildrenButton_Click(sender, null);
            }
        }

        private void serverDisplayPanel_SizeChanged(object sender, EventArgs e)
        {
            resolver.pictureBox.Size = serverImageSize;
            resolver.pictureBox.Location = resolverImageLocation;
            proxy.pictureBox.Size = serverImageSize;
            proxy.pictureBox.Location = proxyImageLocation;

            resolver.label.Location = new Point(resolverImageLocation.X, resolverImageLocation.Y + serverImageSize.Height);
            proxy.label.Location = new Point(proxyImageLocation.X, proxyImageLocation.Y + serverImageSize.Height);
            proxy.label.MinimumSize = resolver.label.MinimumSize = new Size(serverImageSize.Width, 0);

            if (fileservers.Count == 1)
            {
                fileservers[0].pictureBox.Size = serverImageSize;
                fileservers[0].pictureBox.Location = new Point(serverDisplayPanel.Width / 2 - serverImageSize.Width / 2, serverImageY);
            }
            else
            {
                for (int i = 0; i < fileservers.Count; i++)
                {
                    PictureBox pb = fileservers[i].pictureBox;
                    pb.Size = serverImageSize;
                    int w = serverDisplayPanel.Width;
                    // heavy math ahead
                    pb.Location = new Point(w / 8 + i * (w * 3 / 4 - serverImageSize.Width) / (fileservers.Count - 1), serverImageY);
                }
            }
        }

        private void addServerButton_Click(object sender, EventArgs e)
        {
            NetworkEntity server = new NetworkEntity("FileServer");
            server.pictureBox.Click += serverImage_Click;
            fileservers.Add(server);
            server.Start();
            serverDisplayPanel.Controls.Add(server.pictureBox);
            serverDisplayPanel_SizeChanged(sender, e);
        }

        private void killChildrenButton_Click(object sender, EventArgs e)
        {
            addServerButton.Enabled = false;
            serverDisplayPanel.Controls.Clear();

            resolver.Kill();
            proxy.Kill();
            foreach (NetworkEntity server in fileservers)
                server.Kill();
        }

        private void killServerButton_Click(object sender, EventArgs e)
        {
            if (activeServer != null && activeServer != resolver && activeServer != proxy)
            {
                activeServer.Kill();
                killServerButton.Enabled = !(removeServerButton.Enabled = true);
            }
        }

        private void removeServerButton_Click(object sender, EventArgs e)
        {
            if (activeServer != null && activeServer != resolver && activeServer != proxy)
            {
                fileservers.Remove(activeServer);
                serverDisplayPanel.Controls.Remove(activeServer.pictureBox);
                activeServer = null;
                killServerButton.Enabled = removeServerButton.Enabled = false;

                serverDisplayPanel_SizeChanged(sender, e);
            }
        }

        private void logUpdateTimer_Tick(object sender, EventArgs e)
        {
            for (; proxyLogLineParsed < proxy.log.Count; proxyLogLineParsed++)
            {
                string[] elements = proxy.log[proxyLogLineParsed].Split(' ');
                switch (elements[0])
                {
                    case "CONNECT":
                        int socket = Convert.ToInt32(elements[3]);
                        if (elements[1] == "localhost" || elements[1] == "127.0.0.1")
                        {
                            int port = Convert.ToInt32(elements[2]);
                            NetworkEntity server = fileservers.AsEnumerable().FirstOrDefault(x => x.port == port);
                            proxySocketMap.Add(socket, server);
                            if (server != null)
                                proxyConnections.Add(socket, new EntityConnection(proxy, server));
                        }
                        else
                        {
                            proxySocketMap.Add(socket, null);
                        }
                        break;
                    case "CLOSED":
                        socket = Convert.ToInt32(elements[1]);
                        if (proxySocketMap.Keys.Contains(socket))
                        {
                            if (proxySocketMap[socket] != null)
                                proxyConnections.Remove(socket);
                            proxySocketMap.Remove(socket);
                        }
                        break;
                    case "REQUESTED":
                        socket = Convert.ToInt32(elements[2]);
                        proxyConnections[socket].addPacket(new Packet(proxy.log[proxyLogLineParsed]));
                        browserListView.Items.Add(new ListViewItem(new string[]{
                            "...",
                            elements[0],
                            elements[1].EndsWith("?sig") ? "Signature" : "Object",
                            elements[1],
                            (fileservers.IndexOf(proxySocketMap[Convert.ToInt32(elements[2])]) + 1).ToString()
                        }));
                        browserListView.Items[browserListView.Items.Count - 1].EnsureVisible();
                        break;
                    case "COMPLETED":
                        socket = Convert.ToInt32(elements[2]);
                        proxyConnections[socket].addPacket(new Packet(proxy.log[proxyLogLineParsed]));
                        foreach (ListViewItem item in browserListView.Items)
                        {
                            if (item.SubItems[3].Text == elements[1] && item.SubItems[1].Text == "REQUESTED")
                            {
                                item.SubItems[1].Text = elements[0];
                                item.SubItems[0].Text = elements[3];
                                if (elements[3] == "200")
                                    item.BackColor = Color.LightGreen;
                                else
                                    item.BackColor = Color.Red;
                                break;
                            }
                        }
                        break;
                    case "PROCESSED":
                        foreach (ListViewItem item in browserListView.Items)
                        {
                            if (item.SubItems[2].Text == elements[1] || item.SubItems[2].Text == elements[1] + "?sig")
                            {
                                item.SubItems[1].Text = elements[0];
                                item.SubItems[0].Text = elements[3];
                                if (elements[3] == "200")
                                    item.BackColor = Color.LightGreen;
                                else
                                    item.BackColor = Color.Yellow;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        private void browserListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            foreach (var server in fileservers)
            {
                server.pictureBox.BackColor = Color.White;
                if (server == activeServer)
                    server.pictureBox.BackColor = Color.LightBlue;
            }
            foreach (ListViewItem item in browserListView.SelectedItems)
            {
                int server = Convert.ToInt32(item.SubItems[4].Text) - 1;
                fileservers[server].pictureBox.BackColor = Color.PaleGreen;
            }
        }
    }
}
