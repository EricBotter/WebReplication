namespace WebReplicationDemo
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.serverDetailPanel = new System.Windows.Forms.Panel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.killServerButton = new System.Windows.Forms.Button();
            this.removeServerButton = new System.Windows.Forms.Button();
            this.connectionsTreeView = new System.Windows.Forms.TreeView();
            this.logListBox = new System.Windows.Forms.ListBox();
            this.serverDisplayPanel = new System.Windows.Forms.Panel();
            this.controlPanel = new System.Windows.Forms.Panel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.addServerButton = new System.Windows.Forms.Button();
            this.killChildrenButton = new System.Windows.Forms.Button();
            this.browserListView = new System.Windows.Forms.ListView();
            this.statusColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.logEntryColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.typeColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.urlColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.serverColumnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.logUpdateTimer = new System.Windows.Forms.Timer(this.components);
            this.tableLayoutPanel1.SuspendLayout();
            this.serverDetailPanel.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.controlPanel.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 70F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel1.Controls.Add(this.serverDetailPanel, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.serverDisplayPanel, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.controlPanel, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 70F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1503, 1068);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // serverDetailPanel
            // 
            this.serverDetailPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.serverDetailPanel.Controls.Add(this.tableLayoutPanel3);
            this.serverDetailPanel.Controls.Add(this.logListBox);
            this.serverDetailPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.serverDetailPanel.Location = new System.Drawing.Point(1052, 4);
            this.serverDetailPanel.Margin = new System.Windows.Forms.Padding(0, 4, 4, 4);
            this.serverDetailPanel.Name = "serverDetailPanel";
            this.serverDetailPanel.Size = new System.Drawing.Size(447, 739);
            this.serverDetailPanel.TabIndex = 2;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.killServerButton, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.removeServerButton, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.connectionsTreeView, 0, 1);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 2;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(445, 501);
            this.tableLayoutPanel3.TabIndex = 3;
            // 
            // killServerButton
            // 
            this.killServerButton.Enabled = false;
            this.killServerButton.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.killServerButton.Location = new System.Drawing.Point(3, 3);
            this.killServerButton.Name = "killServerButton";
            this.killServerButton.Size = new System.Drawing.Size(216, 60);
            this.killServerButton.TabIndex = 1;
            this.killServerButton.Text = "Kill Server";
            this.killServerButton.UseVisualStyleBackColor = true;
            this.killServerButton.Click += new System.EventHandler(this.killServerButton_Click);
            // 
            // removeServerButton
            // 
            this.removeServerButton.Enabled = false;
            this.removeServerButton.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.removeServerButton.Location = new System.Drawing.Point(225, 3);
            this.removeServerButton.Name = "removeServerButton";
            this.removeServerButton.Size = new System.Drawing.Size(217, 60);
            this.removeServerButton.TabIndex = 2;
            this.removeServerButton.Text = "Remove Server";
            this.removeServerButton.UseVisualStyleBackColor = true;
            this.removeServerButton.Click += new System.EventHandler(this.removeServerButton_Click);
            // 
            // connectionsTreeView
            // 
            this.tableLayoutPanel3.SetColumnSpan(this.connectionsTreeView, 2);
            this.connectionsTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.connectionsTreeView.Location = new System.Drawing.Point(3, 73);
            this.connectionsTreeView.Name = "connectionsTreeView";
            this.connectionsTreeView.Size = new System.Drawing.Size(439, 425);
            this.connectionsTreeView.TabIndex = 3;
            // 
            // logListBox
            // 
            this.logListBox.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.logListBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.logListBox.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.logListBox.FormattingEnabled = true;
            this.logListBox.ItemHeight = 20;
            this.logListBox.Location = new System.Drawing.Point(0, 517);
            this.logListBox.Name = "logListBox";
            this.logListBox.Size = new System.Drawing.Size(445, 220);
            this.logListBox.TabIndex = 0;
            // 
            // serverDisplayPanel
            // 
            this.serverDisplayPanel.BackColor = System.Drawing.Color.White;
            this.serverDisplayPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.serverDisplayPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.serverDisplayPanel.Location = new System.Drawing.Point(4, 4);
            this.serverDisplayPanel.Margin = new System.Windows.Forms.Padding(4);
            this.serverDisplayPanel.Name = "serverDisplayPanel";
            this.serverDisplayPanel.Size = new System.Drawing.Size(1044, 739);
            this.serverDisplayPanel.TabIndex = 1;
            this.serverDisplayPanel.SizeChanged += new System.EventHandler(this.serverDisplayPanel_SizeChanged);
            // 
            // controlPanel
            // 
            this.controlPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tableLayoutPanel1.SetColumnSpan(this.controlPanel, 2);
            this.controlPanel.Controls.Add(this.tableLayoutPanel2);
            this.controlPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.controlPanel.Location = new System.Drawing.Point(4, 747);
            this.controlPanel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 4);
            this.controlPanel.Name = "controlPanel";
            this.controlPanel.Size = new System.Drawing.Size(1495, 317);
            this.controlPanel.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 220F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.browserListView, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(1493, 315);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.addServerButton);
            this.panel1.Controls.Add(this.killChildrenButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(220, 315);
            this.panel1.TabIndex = 0;
            // 
            // addServerButton
            // 
            this.addServerButton.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.addServerButton.Location = new System.Drawing.Point(10, 10);
            this.addServerButton.Name = "addServerButton";
            this.addServerButton.Size = new System.Drawing.Size(200, 60);
            this.addServerButton.TabIndex = 0;
            this.addServerButton.Text = "Add Server";
            this.addServerButton.UseVisualStyleBackColor = true;
            this.addServerButton.Click += new System.EventHandler(this.addServerButton_Click);
            // 
            // killChildrenButton
            // 
            this.killChildrenButton.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.killChildrenButton.Location = new System.Drawing.Point(10, 76);
            this.killChildrenButton.Name = "killChildrenButton";
            this.killChildrenButton.Size = new System.Drawing.Size(200, 60);
            this.killChildrenButton.TabIndex = 1;
            this.killChildrenButton.Text = "Terminate";
            this.killChildrenButton.UseVisualStyleBackColor = true;
            this.killChildrenButton.Click += new System.EventHandler(this.killChildrenButton_Click);
            // 
            // browserListView
            // 
            this.browserListView.AutoArrange = false;
            this.browserListView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.browserListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.statusColumnHeader,
            this.logEntryColumnHeader,
            this.typeColumnHeader,
            this.urlColumnHeader,
            this.serverColumnHeader});
            this.browserListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.browserListView.FullRowSelect = true;
            this.browserListView.GridLines = true;
            this.browserListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.browserListView.Location = new System.Drawing.Point(220, 0);
            this.browserListView.Margin = new System.Windows.Forms.Padding(0);
            this.browserListView.Name = "browserListView";
            this.browserListView.ShowGroups = false;
            this.browserListView.Size = new System.Drawing.Size(1273, 315);
            this.browserListView.TabIndex = 1;
            this.browserListView.UseCompatibleStateImageBehavior = false;
            this.browserListView.View = System.Windows.Forms.View.Details;
            this.browserListView.SelectedIndexChanged += new System.EventHandler(this.browserListView_SelectedIndexChanged);
            // 
            // statusColumnHeader
            // 
            this.statusColumnHeader.Text = "Status";
            // 
            // logEntryColumnHeader
            // 
            this.logEntryColumnHeader.Text = "Log Entry";
            this.logEntryColumnHeader.Width = 250;
            // 
            // typeColumnHeader
            // 
            this.typeColumnHeader.Text = "Type";
            this.typeColumnHeader.Width = 160;
            // 
            // urlColumnHeader
            // 
            this.urlColumnHeader.Text = "URL";
            this.urlColumnHeader.Width = 560;
            // 
            // serverColumnHeader
            // 
            this.serverColumnHeader.Text = "Server";
            this.serverColumnHeader.Width = 68;
            // 
            // logUpdateTimer
            // 
            this.logUpdateTimer.Tick += new System.EventHandler(this.logUpdateTimer_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(1503, 1068);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MinimumSize = new System.Drawing.Size(1489, 1097);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Web Replication - Demo";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.serverDetailPanel.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.controlPanel.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel serverDetailPanel;
        private System.Windows.Forms.Panel serverDisplayPanel;
        private System.Windows.Forms.Panel controlPanel;
        private System.Windows.Forms.Button addServerButton;
        private System.Windows.Forms.ListBox logListBox;
        private System.Windows.Forms.Button killChildrenButton;
        private System.Windows.Forms.Button killServerButton;
        private System.Windows.Forms.Button removeServerButton;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ListView browserListView;
        private System.Windows.Forms.ColumnHeader statusColumnHeader;
        private System.Windows.Forms.ColumnHeader logEntryColumnHeader;
        private System.Windows.Forms.ColumnHeader urlColumnHeader;
        private System.Windows.Forms.Timer logUpdateTimer;
        private System.Windows.Forms.ColumnHeader serverColumnHeader;
        private System.Windows.Forms.ColumnHeader typeColumnHeader;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.TreeView connectionsTreeView;


    }
}

