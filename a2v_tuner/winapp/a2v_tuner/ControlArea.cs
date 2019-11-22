using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace a2v_tuner
{
    class ControlArea : Panel
    {
        private Form1 form1 = null;
        private Port port1 = null;
        private Button btnConnect = new Button();
        public TextBox tbInfo = new TextBox();

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // ControlArea
            // 
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ControlArea_Paint);
            this.ResumeLayout(false);
        }

        public ControlArea(Form1 form, Port port)
        {
            this.form1 = form;
            this.port1 = port;

            InitializeComponent();

            this.BackColor = SystemColors.Window;

            Controls.Add(btnConnect);
            btnConnect.Text = Properties.Resources.BTN_TEXT_CONNECT;
            btnConnect.Click += new EventHandler(this.btnConnect_Click);

            Controls.Add(tbInfo);
            tbInfo.Font = new Font("Courier New", 11f);
            tbInfo.WordWrap = false;
            tbInfo.ReadOnly = true;
            tbInfo.BorderStyle = BorderStyle.None;
            tbInfo.BackColor = SystemColors.Window;
            tbInfo.TabStop = false;
        }

        private void ControlArea_Paint(object sender, PaintEventArgs e)
        {
            InitializeControls();
        }

        private void InitializeControls()
        {
            int cPosX = 0;
            int cPosY = 0;
            btnConnect.Location = new Point(cPosX, cPosY);
            btnConnect.Size = new Size(this.Width / 8, this.Height);
            cPosX += btnConnect.Width;

            tbInfo.Location = new Point(cPosX + 8, cPosY + 8);
            tbInfo.Size = new Size(this.Width / 2, this.Height);
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {            
            if(btnConnect.Text.Equals(Properties.Resources.BTN_TEXT_CONNECT))
            {
                btnConnect.Enabled = false;
                infoMessage("Connecting..");

                if(port1.Connect(921600) != -1)
                {
                    if(port1.Check_Connection() != -1) {
                        infoMessage("Connected.");
                        btnConnect.Text = Properties.Resources.BTN_TEXT_DISCONNECT;

                        form1.loadControlValues();
                        form1.enableAreas(true);
                    }
                    else
                    {
                        port1.delay(500);
                        port1.Close();
                        infoMessage("Device Error.");
                    }
                }
                else
                {
                    infoMessage("Failed to Connect.");
                    btnConnect.Text = Properties.Resources.BTN_TEXT_CONNECT;

                    form1.enableAreas(false);
                }

                btnConnect.Enabled = true;
            }
            else
            {
                btnConnect.Enabled = false;

                if(form1.isPlay())
                {
                    form1.playStop();
                }

                port1.delay(500);
                port1.Close();
                infoMessage("Disconnected.");

                btnConnect.Text = Properties.Resources.BTN_TEXT_CONNECT;

                form1.enableAreas(false);

                btnConnect.Enabled = true;
            }
        }

        public void enableBtnConnect(bool enable)
        {
            btnConnect.Enabled = enable;
        }

        public void infoMessage(string msg)
        {
            tbInfo.Text = msg;
        }
    }
}
