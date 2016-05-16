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
    public partial class Form1 : Form
    {
        NetworkEntity resolver, proxy;
        List<NetworkEntity> fileservers = new List<NetworkEntity>();

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

        public Form1()
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
            logListBox.SuspendLayout();
            logListBox.Items.Clear();
            logListBox.Items.AddRange(proxy.log.ToArray());
            logListBox.ResumeLayout();
        }
        void resolverImage_Click(object sender, EventArgs e)
        {
            logListBox.SuspendLayout();
            logListBox.Items.Clear();
            logListBox.Items.AddRange(resolver.log.ToArray());
            logListBox.ResumeLayout();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            resolver.Start();
            proxy.Start();

            serverDisplayPanel_SizeChanged(sender, e);
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
    }
}
