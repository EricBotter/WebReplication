using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WebReplicationDemo
{
    class NetworkEntity
    {
        private delegate void ThreadReadingDelegate();

        public PictureBox pictureBox { get; private set; }
        public Label label { get; private set; }
        public List<string> log { get; private set; }
        public string name { get; private set; }

        private Process process;
        private Thread logReader;
        private ProcessStartInfo psi;

        public NetworkEntity(string name)
        {
            this.name = name;

            psi = new ProcessStartInfo(Properties.Settings.Default.rootPath + "/" + name + ".exe");
            psi.RedirectStandardInput = true;
            psi.RedirectStandardOutput = true;
            psi.RedirectStandardError = true;
            psi.UseShellExecute = false;
            psi.CreateNoWindow = true;

            pictureBox = new PictureBox();
            pictureBox.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox.BackColor = Color.Transparent;
            pictureBox.BorderStyle = BorderStyle.None;
            pictureBox.Image = Properties.Resources.serverImageTrasp;
            pictureBox.Cursor = Cursors.Help;

            label = new Label();
            label.Text = name;
            label.TextAlign = ContentAlignment.TopCenter;
            label.AutoSize = true;
            label.Font = new System.Drawing.Font("Arial", 14);
            label.Cursor = Cursors.Help;
        }

        public void Start()
        {
            process = Process.Start(psi);
            logReader = new Thread(new ThreadStart(LogReaderMethod));
            logReader.Start();
        }

        public void Kill()
        {
            if (!process.HasExited)
                process.Kill();
        }

        private void LogReaderMethod()
        {
            log = new List<string>();
            string line;
            while ((line = process.StandardOutput.ReadLine()) != null)
                log.Add(line);
        }
    }
}
