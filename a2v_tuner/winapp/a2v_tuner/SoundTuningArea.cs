using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace a2v_tuner
{
    class SoundTuningArea : Panel
    {
        private Form1 form1 = null;
        private Port port1 = null;

        public Button btnPlay = new Button();
        private Button btnSave = new Button();
        private MultiTrackbar mTrackbarFreqband = new MultiTrackbar();
        private Label labelFreqband = new Label();
        
        private Label labelVdclamp = new Label();
        private Label labelThreshold = new Label();
        private Label label200Hz = new Label();
        private Label label400Hz = new Label();
        private Label label800Hz = new Label();
        private Label label1600Hz = new Label();
        private Label label3200Hz = new Label();
        private Label label6400Hz = new Label();
        private TrackBar trackbarVdclamp = new TrackBar();
        private TrackBar trackbarThreshold = new TrackBar();
        private TrackBar trackbar200Hz = new TrackBar();
        private TrackBar trackbar400Hz = new TrackBar();
        private TrackBar trackbar800Hz = new TrackBar();
        private TrackBar trackbar1600Hz = new TrackBar();
        private TrackBar trackbar3200Hz = new TrackBar();
        private TrackBar trackbar6400Hz = new TrackBar();
        private TextBox textboxVdclamp = new TextBox();
        private TextBox textboxThreshold = new TextBox();
        private TextBox textbox200Hz = new TextBox();
        private TextBox textbox400Hz = new TextBox();
        private TextBox textbox800Hz = new TextBox();
        private TextBox textbox1600Hz = new TextBox();
        private TextBox textbox3200Hz = new TextBox();
        private TextBox textbox6400Hz = new TextBox();
        
        private enum TuningFactor
        {
            VDCLAMP,
            AUTOBRAKE,
            PREFFTGAIN,
            LRGAPCUTTING,
            THRESHOLD,
            CNTFREQLOW,
            CNTFREQMID,
            CNTFREQHIGH,
            EQFREQ200,
            EQFREQ400,
            EQFREQ800,
            EQFREQ1600,
            EQFREQ3200,
            EQFREQ6400,
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // SoundTuningArea
            // 
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.SoundTuningArea_Paint);
            this.ResumeLayout(false);

            Controls.Add(btnPlay);
            btnPlay.Text = Properties.Resources.BTN_TEXT_PLAY;
            btnPlay.Click += new EventHandler(btnPlay_Click);

            Controls.Add(btnSave);
            btnSave.Text = Properties.Resources.BTN_TEXT_SAVE;
            btnSave.Click += new EventHandler(btnSave_Click);

            Controls.Add(mTrackbarFreqband);
            mTrackbarFreqband.Paint += new PaintEventHandler(mTrackbarFreqband_Paint);
            mTrackbarFreqband.MouseUp += new MouseEventHandler(mTrackbarFreqband_MouseUp);
            mTrackbarFreqband.Min = 0;
            mTrackbarFreqband.Max = 69;
            mTrackbarFreqband.SelectedMin = 5;
            mTrackbarFreqband.Value = 18;
            mTrackbarFreqband.SelectedMax = 69;

            Controls.Add(labelFreqband);
            labelFreqband.Font = new Font("Courier New", 8f);

            Controls.Add(labelVdclamp);
            labelVdclamp.Text = "Vdclamp";
            labelVdclamp.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(labelThreshold);
            labelThreshold.Text = "Threshold";
            labelThreshold.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label200Hz);
            label200Hz.Text = "200Hz";
            label200Hz.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label400Hz);
            label400Hz.Text = "400Hz";
            label400Hz.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label800Hz);
            label800Hz.Text = "800Hz";
            label800Hz.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label1600Hz);
            label1600Hz.Text = "1600Hz";
            label1600Hz.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label3200Hz);
            label3200Hz.Text = "3200Hz";
            label3200Hz.TextAlign = ContentAlignment.MiddleCenter;
            Controls.Add(label6400Hz);
            label6400Hz.Text = "6400Hz";
            label6400Hz.TextAlign = ContentAlignment.MiddleCenter;

            Controls.Add(trackbarVdclamp);
            trackbarVdclamp.Maximum = 255;
            trackbarVdclamp.Minimum = 0;
            trackbarVdclamp.TickFrequency = 16;
            trackbarVdclamp.TickStyle = TickStyle.Both;
            trackbarVdclamp.Orientation = Orientation.Vertical;
            trackbarVdclamp.Scroll += new EventHandler(trackbarVdclamp_Scroll);
            trackbarVdclamp.MouseUp += new MouseEventHandler(trackbarVdclamp_MouseUp);
            trackbarVdclamp.MouseWheel += new MouseEventHandler(trackbarVdclamp_MouseWheel);
            Controls.Add(trackbarThreshold);
            trackbarThreshold.Maximum = 255;
            trackbarThreshold.Minimum = 0;
            trackbarThreshold.TickFrequency = 16;
            trackbarThreshold.TickStyle = TickStyle.Both;
            trackbarThreshold.Orientation = Orientation.Vertical;
            trackbarThreshold.Scroll += new EventHandler(trackbarThreshold_Scroll);
            trackbarThreshold.MouseUp += new MouseEventHandler(trackbarThreshold_MouseUp);
            trackbarThreshold.MouseWheel += new MouseEventHandler(trackbarThreshold_MouseWheel);
            Controls.Add(trackbar200Hz);
            trackbar200Hz.Maximum = 255;
            trackbar200Hz.Minimum = 0;
            trackbar200Hz.TickFrequency = 16;
            trackbar200Hz.TickStyle = TickStyle.Both;
            trackbar200Hz.Orientation = Orientation.Vertical;
            trackbar200Hz.Scroll += new EventHandler(trackbar200Hz_Scroll);
            trackbar200Hz.MouseUp += new MouseEventHandler(trackbar200Hz_MouseUp);
            trackbar200Hz.MouseWheel += new MouseEventHandler(trackbar200Hz_MouseWheel);
            Controls.Add(trackbar400Hz);
            trackbar400Hz.Maximum = 255;
            trackbar400Hz.Minimum = 0;
            trackbar400Hz.TickFrequency = 16;
            trackbar400Hz.TickStyle = TickStyle.Both;
            trackbar400Hz.Orientation = Orientation.Vertical;
            trackbar400Hz.Scroll += new EventHandler(trackbar400Hz_Scroll);
            trackbar400Hz.MouseUp += new MouseEventHandler(trackbar400Hz_MouseUp);
            trackbar400Hz.MouseWheel += new MouseEventHandler(trackbar400Hz_MouseWheel);
            Controls.Add(trackbar800Hz);
            trackbar800Hz.Maximum = 255;
            trackbar800Hz.Minimum = 0;
            trackbar800Hz.TickFrequency = 16;
            trackbar800Hz.TickStyle = TickStyle.Both;
            trackbar800Hz.Orientation = Orientation.Vertical;
            trackbar800Hz.Scroll += new EventHandler(trackbar800Hz_Scroll);
            trackbar800Hz.MouseUp += new MouseEventHandler(trackbar800Hz_MouseUp);
            trackbar800Hz.MouseWheel += new MouseEventHandler(trackbar800Hz_MouseWheel);
            Controls.Add(trackbar1600Hz);
            trackbar1600Hz.Maximum = 255;
            trackbar1600Hz.Minimum = 0;
            trackbar1600Hz.TickFrequency = 16;
            trackbar1600Hz.TickStyle = TickStyle.Both;
            trackbar1600Hz.Orientation = Orientation.Vertical;
            trackbar1600Hz.Scroll += new EventHandler(trackbar1600Hz_Scroll);
            trackbar1600Hz.MouseUp += new MouseEventHandler(trackbar1600Hz_MouseUp);
            trackbar1600Hz.MouseWheel += new MouseEventHandler(trackbar1600Hz_MouseWheel);
            Controls.Add(trackbar3200Hz);
            trackbar3200Hz.Maximum = 255;
            trackbar3200Hz.Minimum = 0;
            trackbar3200Hz.TickFrequency = 16;
            trackbar3200Hz.TickStyle = TickStyle.Both;
            trackbar3200Hz.Orientation = Orientation.Vertical;
            trackbar3200Hz.Scroll += new EventHandler(trackbar3200Hz_Scroll);
            trackbar3200Hz.MouseUp += new MouseEventHandler(trackbar3200Hz_MouseUp);
            trackbar3200Hz.MouseWheel += new MouseEventHandler(trackbar3200Hz_MouseWheel);
            Controls.Add(trackbar6400Hz);
            trackbar6400Hz.Maximum = 255;
            trackbar6400Hz.Minimum = 0;
            trackbar6400Hz.TickFrequency = 16;
            trackbar6400Hz.TickStyle = TickStyle.Both;
            trackbar6400Hz.Orientation = Orientation.Vertical;
            trackbar6400Hz.Scroll += new EventHandler(trackbar6400Hz_Scroll);
            trackbar6400Hz.MouseUp += new MouseEventHandler(trackbar6400Hz_MouseUp);
            trackbar6400Hz.MouseWheel += new MouseEventHandler(trackbar6400Hz_MouseWheel);

            Controls.Add(textboxVdclamp);
            textboxVdclamp.Text = "0";
            textboxVdclamp.Click += new EventHandler(textboxVdclamp_Click);
            textboxVdclamp.TextChanged += new EventHandler(textboxVdclamp_TextChanged);
            textboxVdclamp.KeyPress += new KeyPressEventHandler(textboxVdclamp_KeyPress);
            Controls.Add(textboxThreshold);
            textboxThreshold.Text = "0";
            textboxThreshold.Click += new EventHandler(textboxThreshold_Click);
            textboxThreshold.TextChanged += new EventHandler(textboxThreshold_TextChanged);
            textboxThreshold.KeyPress += new KeyPressEventHandler(textboxThreshold_KeyPress);
            Controls.Add(textbox200Hz);
            textbox200Hz.Text = "0";
            textbox200Hz.Click += new EventHandler(textbox200Hz_Click);
            textbox200Hz.TextChanged += new EventHandler(textbox200Hz_TextChanged);
            textbox200Hz.KeyPress += new KeyPressEventHandler(textbox200Hz_KeyPress);
            Controls.Add(textbox400Hz);
            textbox400Hz.Text = "0";
            textbox400Hz.Click += new EventHandler(textbox400Hz_Click);
            textbox400Hz.TextChanged += new EventHandler(textbox400Hz_TextChanged);
            textbox400Hz.KeyPress += new KeyPressEventHandler(textbox400Hz_KeyPress);
            Controls.Add(textbox800Hz);
            textbox800Hz.Text = "0";
            textbox800Hz.Click += new EventHandler(textbox800Hz_Click);
            textbox800Hz.TextChanged += new EventHandler(textbox800Hz_TextChanged);
            textbox800Hz.KeyPress += new KeyPressEventHandler(textbox800Hz_KeyPress);
            Controls.Add(textbox1600Hz);
            textbox1600Hz.Text = "0";
            textbox1600Hz.Click += new EventHandler(textbox1600Hz_Click);
            textbox1600Hz.TextChanged += new EventHandler(textbox1600Hz_TextChanged);
            textbox1600Hz.KeyPress += new KeyPressEventHandler(textbox1600Hz_KeyPress);
            Controls.Add(textbox3200Hz);
            textbox3200Hz.Text = "0";
            textbox3200Hz.Click += new EventHandler(textbox3200Hz_Click);
            textbox3200Hz.TextChanged += new EventHandler(textbox3200Hz_TextChanged);
            textbox3200Hz.KeyPress += new KeyPressEventHandler(textbox3200Hz_KeyPress);
            Controls.Add(textbox6400Hz);
            textbox6400Hz.Text = "0";
            textbox6400Hz.Click += new EventHandler(textbox6400Hz_Click);
            textbox6400Hz.TextChanged += new EventHandler(textbox6400Hz_TextChanged);
            textbox6400Hz.KeyPress += new KeyPressEventHandler(textbox6400Hz_KeyPress);

        }

        public SoundTuningArea(Form1 form, Port port)
        {
            this.form1 = form;
            this.port1 = port;

            InitializeComponent();

            this.BackColor = SystemColors.Window;
        }

        private void SoundTuningArea_Paint(object sender, PaintEventArgs e)
        {
            InitializeControls();
        }

        private void InitializeControls()
        {
            int cPosX = 0;
            int cPosY = 0;
            btnPlay.Location = new Point(cPosX, cPosY);
            btnPlay.Size = new Size(this.Width / 8, this.Height / 8);
            cPosX += btnPlay.Width;

            btnSave.Location = new Point(cPosX, cPosY);
            btnSave.Size = new Size(this.Width / 8, this.Height / 8);
            cPosX += btnSave.Width;

            mTrackbarFreqband.Location = new Point(cPosX, cPosY);
            mTrackbarFreqband.Size = new Size(this.Width *3/ 4, this.Height / 16);
            cPosY += mTrackbarFreqband.Height;

            labelFreqband.Location = new Point(cPosX, cPosY);
            labelFreqband.Size = new Size(this.Width *3/ 4, this.Height / 16);
            cPosY += labelFreqband.Height;

            cPosX = 0;
            labelVdclamp.Location = new Point(cPosX, cPosY);
            labelVdclamp.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += labelVdclamp.Width;
            labelThreshold.Location = new Point(cPosX, cPosY);
            labelThreshold.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += labelThreshold.Width;
            label200Hz.Location = new Point(cPosX, cPosY);
            label200Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label200Hz.Width;
            label400Hz.Location = new Point(cPosX, cPosY);
            label400Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label400Hz.Width;
            label800Hz.Location = new Point(cPosX, cPosY);
            label800Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label800Hz.Width;
            label1600Hz.Location = new Point(cPosX, cPosY);
            label1600Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label1600Hz.Width;
            label3200Hz.Location = new Point(cPosX, cPosY);
            label3200Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label3200Hz.Width;
            label6400Hz.Location = new Point(cPosX, cPosY);
            label6400Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += label6400Hz.Width;

            cPosY += labelVdclamp.Height;

            cPosX = 0;
            trackbarVdclamp.Location = new Point((labelVdclamp.Width - trackbarVdclamp.Width) / 2 + cPosX, cPosY);
            trackbarVdclamp.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += labelVdclamp.Width;
            trackbarThreshold.Location = new Point((labelThreshold.Width - trackbarThreshold.Width) / 2 + cPosX, cPosY);
            trackbarThreshold.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += labelThreshold.Width;
            trackbar200Hz.Location = new Point((label200Hz.Width - trackbar200Hz.Width) / 2 + cPosX, cPosY);
            trackbar200Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label200Hz.Width;
            trackbar400Hz.Location = new Point((label400Hz.Width - trackbar400Hz.Width) / 2 + cPosX, cPosY);
            trackbar400Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label400Hz.Width;
            trackbar800Hz.Location = new Point((label800Hz.Width - trackbar800Hz.Width) / 2 + cPosX, cPosY);
            trackbar800Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label800Hz.Width;
            trackbar1600Hz.Location = new Point((label1600Hz.Width - trackbar1600Hz.Width) / 2 + cPosX, cPosY);
            trackbar1600Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label1600Hz.Width;
            trackbar3200Hz.Location = new Point((label3200Hz.Width - trackbar3200Hz.Width) / 2 + cPosX, cPosY);
            trackbar3200Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label3200Hz.Width;
            trackbar6400Hz.Location = new Point((label6400Hz.Width - trackbar6400Hz.Width) / 2 + cPosX, cPosY);
            trackbar6400Hz.Size = new Size(this.Width / 8, this.Height *5 / 8);
            cPosX += label6400Hz.Width;

            cPosY += trackbarVdclamp.Height;

            cPosX = 0;
            textboxVdclamp.Location = new Point(cPosX, cPosY);
            textboxVdclamp.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textboxVdclamp.Width;
            textboxThreshold.Location = new Point(cPosX, cPosY);
            textboxThreshold.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textboxThreshold.Width;
            textbox200Hz.Location = new Point(cPosX, cPosY);
            textbox200Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox200Hz.Width;
            textbox400Hz.Location = new Point(cPosX, cPosY);
            textbox400Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox400Hz.Width;
            textbox800Hz.Location = new Point(cPosX, cPosY);
            textbox800Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox800Hz.Width;
            textbox1600Hz.Location = new Point(cPosX, cPosY);
            textbox1600Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox1600Hz.Width;
            textbox3200Hz.Location = new Point(cPosX, cPosY);
            textbox3200Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox3200Hz.Width;
            textbox6400Hz.Location = new Point(cPosX, cPosY);
            textbox6400Hz.Size = new Size(this.Width / 8, this.Height / 16);
            cPosX += textbox6400Hz.Width;
        }

        public void loadControlValues()
        {
            mTrackbarFreqband.SelectedMin = form1.getParam((byte)TuningFactor.CNTFREQLOW);
            mTrackbarFreqband.Value = form1.getParam((byte)TuningFactor.CNTFREQMID);
            mTrackbarFreqband.SelectedMax = form1.getParam((byte)TuningFactor.CNTFREQHIGH);

            trackbarVdclamp.Value = form1.getParam((byte)TuningFactor.VDCLAMP);
            textboxVdclamp.Text = trackbarVdclamp.Value.ToString();
            labelVdclamp.Text = string.Format("{0,2:f2}V",(double)trackbarVdclamp.Value*0.04);
            form1.VdClamp2VoltMid((byte)trackbarVdclamp.Value);

            trackbarThreshold.Value = form1.getParam((byte)TuningFactor.THRESHOLD);
            textboxThreshold.Text = trackbarThreshold.Value.ToString();
            form1.updateGraphThreshold((byte)trackbarThreshold.Value);

            trackbar200Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ200);
            textbox200Hz.Text = trackbar200Hz.Value.ToString();
            trackbar400Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ400);
            textbox400Hz.Text = trackbar400Hz.Value.ToString();
            trackbar800Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ800);
            textbox800Hz.Text = trackbar800Hz.Value.ToString();
            trackbar1600Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ1600);
            textbox1600Hz.Text = trackbar1600Hz.Value.ToString();
            trackbar3200Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ3200);
            textbox3200Hz.Text = trackbar3200Hz.Value.ToString();
            trackbar6400Hz.Value = form1.getParam((byte)TuningFactor.EQFREQ6400);
            textbox6400Hz.Text = trackbar6400Hz.Value.ToString();
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            if(form1.isPlay())
            {
                form1.playStop();
                btnSave.Enabled = true;
                form1.enableActuatorArea(true);
            }
            else
            {
                form1.playStart();
                btnSave.Enabled = false;
                form1.enableActuatorArea(false);
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            form1.recordParams();
        }

        protected void mTrackbarFreqband_Paint(object sender, PaintEventArgs e)
        {
            string strFreqband = string.Format("0.00 --L-- {0,6:f2} --M-- {1,6:f2} --H-- {2,6:f2} Hz",
                                                mTrackbarFreqband.SelectedMin * 93.75,
                                                mTrackbarFreqband.Value * 93.75,
                                                mTrackbarFreqband.SelectedMax * 93.75);
            labelFreqband.Text = strFreqband;
        }

        protected void mTrackbarFreqband_MouseUp(object sender, MouseEventArgs e)
        {
            switch(mTrackbarFreqband.getMovingMode())
            {
                case 1:
                    form1.setParams((byte)TuningFactor.CNTFREQLOW, (byte)mTrackbarFreqband.SelectedMin);
                    break;
                case 2:
                    form1.setParams((byte)TuningFactor.CNTFREQMID, (byte)mTrackbarFreqband.Value);
                    break;
                case 3:
                    form1.setParams((byte)TuningFactor.CNTFREQHIGH, (byte)mTrackbarFreqband.SelectedMax);
                    break;
            }
        }

        private void setVdclamp(byte vdclamp)
        {
            labelVdclamp.Text = string.Format("{0,2:f2}V",(double)vdclamp*0.04);
            form1.setParams((byte)TuningFactor.VDCLAMP, vdclamp);
            form1.VdClamp2VoltMid(vdclamp);            
        }

        private void trackbarVdclamp_Scroll(object sender, EventArgs e)
        {
            textboxVdclamp.Text = trackbarVdclamp.Value.ToString();
            labelVdclamp.Text = string.Format("{0,2:f2}V",(double)trackbarVdclamp.Value*0.04);
        }

        private void trackbarVdclamp_MouseUp(object sender, MouseEventArgs e)
        {
            setVdclamp((byte)trackbarVdclamp.Value);
        }

        private void trackbarVdclamp_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            if (e.Delta > 0)
            {
                if (trackbarVdclamp.Value < trackbarVdclamp.Maximum)
                {
                    trackbarVdclamp.Value++;
                }
            }
            else
            {
                if (trackbarVdclamp.Value > trackbarVdclamp.Minimum)
                {
                    trackbarVdclamp.Value--;
                }
            }
            textboxVdclamp.Text = trackbarVdclamp.Value.ToString();
            setVdclamp((byte)trackbarVdclamp.Value);
        }

        private void textboxVdclamp_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textboxVdclamp_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textboxVdclamp.Text))
            {
                int val = int.Parse(textboxVdclamp.Text);
                if (val > 255)
                {
                    textboxVdclamp.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textboxVdclamp.Text = "0";
                    val = 0;
                }
                trackbarVdclamp.Value = val;
                labelVdclamp.Text = string.Format("{0,2:f2}V",(double)val*0.04);
            }
        }

        private void textboxVdclamp_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                setVdclamp((byte)trackbarVdclamp.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbarThreshold_Scroll(object sender, EventArgs e)
        {
            textboxThreshold.Text = trackbarThreshold.Value.ToString();
            form1.updateGraphThreshold((byte)trackbarThreshold.Value);
        }

        private void trackbarThreshold_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.THRESHOLD, (byte)trackbarThreshold.Value);
        }

        private void trackbarThreshold_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbarThreshold.Value;
            if (e.Delta > 0)
            {
                if (trackbarThreshold.Value < trackbarThreshold.Maximum)
                {
                    value += 4;
                    if (value > trackbarThreshold.Maximum) value = trackbarThreshold.Maximum;
                }
            }
            else
            {
                if (trackbarThreshold.Value > trackbarThreshold.Minimum)
                {
                    value -= 4;
                    if (value < trackbarThreshold.Minimum) value = trackbarThreshold.Minimum;
                }
            }
            trackbarThreshold.Value = value;
            textboxThreshold.Text = trackbarThreshold.Value.ToString();
            form1.setParams((byte)TuningFactor.THRESHOLD, (byte)trackbarThreshold.Value);
            form1.updateGraphThreshold((byte)trackbarThreshold.Value);
        }

        private void textboxThreshold_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textboxThreshold_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textboxThreshold.Text))
            {
                int val = int.Parse(textboxThreshold.Text);
                if (val > 255)
                {
                    textboxThreshold.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textboxThreshold.Text = "0";
                    val = 0;
                }
                trackbarThreshold.Value = val;
            }
        }

        private void textboxThreshold_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.THRESHOLD, (byte)trackbarThreshold.Value);
                form1.updateGraphThreshold((byte)trackbarThreshold.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar200Hz_Scroll(object sender, EventArgs e)
        {
            textbox200Hz.Text = trackbar200Hz.Value.ToString();
        }

        private void trackbar200Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ200, (byte)trackbar200Hz.Value);
        }

        private void trackbar200Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar200Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar200Hz.Value < trackbar200Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar200Hz.Maximum) value = trackbar200Hz.Maximum;
                }
            }
            else
            {
                if (trackbar200Hz.Value > trackbar200Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar200Hz.Minimum) value = trackbar200Hz.Minimum;
                }
            }
            trackbar200Hz.Value = value;
            textbox200Hz.Text = trackbar200Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ200, (byte)trackbar200Hz.Value);
        }

        private void textbox200Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox200Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox200Hz.Text))
            {
                int val = int.Parse(textbox200Hz.Text);
                if (val > 255)
                {
                    textbox200Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox200Hz.Text = "0";
                    val = 0;
                }
                trackbar200Hz.Value = val;
            }
        }

        private void textbox200Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ200, (byte)trackbar200Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar400Hz_Scroll(object sender, EventArgs e)
        {
            textbox400Hz.Text = trackbar400Hz.Value.ToString();
        }

        private void trackbar400Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ400, (byte)trackbar400Hz.Value);
        }

        private void trackbar400Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar400Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar400Hz.Value < trackbar400Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar400Hz.Maximum) value = trackbar400Hz.Maximum;
                }
            }
            else
            {
                if (trackbar400Hz.Value > trackbar400Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar400Hz.Minimum) value = trackbar400Hz.Minimum;
                }
            }
            trackbar400Hz.Value = value;
            textbox400Hz.Text = trackbar400Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ400, (byte)trackbar400Hz.Value);
        }

        private void textbox400Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox400Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox400Hz.Text))
            {
                int val = int.Parse(textbox400Hz.Text);
                if (val > 255)
                {
                    textbox400Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox400Hz.Text = "0";
                    val = 0;
                }
                trackbar400Hz.Value = val;
            }
        }

        private void textbox400Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ400, (byte)trackbar400Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar800Hz_Scroll(object sender, EventArgs e)
        {
            textbox800Hz.Text = trackbar800Hz.Value.ToString();
        }

        private void trackbar800Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ800, (byte)trackbar800Hz.Value);
        }

        private void trackbar800Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar800Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar800Hz.Value < trackbar800Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar800Hz.Maximum) value = trackbar800Hz.Maximum;
                }
            }
            else
            {
                if (trackbar800Hz.Value > trackbar800Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar800Hz.Minimum) value = trackbar800Hz.Minimum;
                }
            }
            trackbar800Hz.Value = value;
            textbox800Hz.Text = trackbar800Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ800, (byte)trackbar800Hz.Value);
        }

        private void textbox800Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox800Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox800Hz.Text))
            {
                int val = int.Parse(textbox800Hz.Text);
                if (val > 255)
                {
                    textbox800Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox800Hz.Text = "0";
                    val = 0;
                }
                trackbar800Hz.Value = val;
            }
        }

        private void textbox800Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ800, (byte)trackbar800Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar1600Hz_Scroll(object sender, EventArgs e)
        {
            textbox1600Hz.Text = trackbar1600Hz.Value.ToString();
        }

        private void trackbar1600Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ1600, (byte)trackbar1600Hz.Value);
        }

        private void trackbar1600Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar1600Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar1600Hz.Value < trackbar1600Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar1600Hz.Maximum) value = trackbar1600Hz.Maximum;
                }
            }
            else
            {
                if (trackbar1600Hz.Value > trackbar1600Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar1600Hz.Minimum) value = trackbar1600Hz.Minimum;
                }
            }
            trackbar1600Hz.Value = value;
            textbox1600Hz.Text = trackbar1600Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ1600, (byte)trackbar1600Hz.Value);
        }

        private void textbox1600Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox1600Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox1600Hz.Text))
            {
                int val = int.Parse(textbox1600Hz.Text);
                if (val > 255)
                {
                    textbox1600Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox1600Hz.Text = "0";
                    val = 0;
                }
                trackbar1600Hz.Value = val;
            }
        }

        private void textbox1600Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ1600, (byte)trackbar1600Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar3200Hz_Scroll(object sender, EventArgs e)
        {
            textbox3200Hz.Text = trackbar3200Hz.Value.ToString();
        }

        private void trackbar3200Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ3200, (byte)trackbar3200Hz.Value);
        }

        private void trackbar3200Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar3200Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar3200Hz.Value < trackbar3200Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar3200Hz.Maximum) value = trackbar3200Hz.Maximum;
                }
            }
            else
            {
                if (trackbar3200Hz.Value > trackbar3200Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar3200Hz.Minimum) value = trackbar3200Hz.Minimum;
                }
            }
            trackbar3200Hz.Value = value;
            textbox3200Hz.Text = trackbar3200Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ3200, (byte)trackbar3200Hz.Value);
        }

        private void textbox3200Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox3200Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox3200Hz.Text))
            {
                int val = int.Parse(textbox3200Hz.Text);
                if (val > 255)
                {
                    textbox3200Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox3200Hz.Text = "0";
                    val = 0;
                }
                trackbar3200Hz.Value = val;
            }
        }

        private void textbox3200Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ3200, (byte)trackbar3200Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void trackbar6400Hz_Scroll(object sender, EventArgs e)
        {
            textbox6400Hz.Text = trackbar6400Hz.Value.ToString();
        }

        private void trackbar6400Hz_MouseUp(object sender, MouseEventArgs e)
        {
            form1.setParams((byte)TuningFactor.EQFREQ6400, (byte)trackbar6400Hz.Value);
        }

        private void trackbar6400Hz_MouseWheel(object sender, MouseEventArgs e)
        {
            ((HandledMouseEventArgs)e).Handled = true;
            int value = trackbar6400Hz.Value;
            if (e.Delta > 0)
            {
                if (trackbar6400Hz.Value < trackbar6400Hz.Maximum)
                {
                    value += 16;
                    if (value > trackbar6400Hz.Maximum) value = trackbar6400Hz.Maximum;
                }
            }
            else
            {
                if (trackbar6400Hz.Value > trackbar6400Hz.Minimum)
                {
                    value -= 16;
                    if (value < trackbar6400Hz.Minimum) value = trackbar6400Hz.Minimum;
                }
            }
            trackbar6400Hz.Value = value;
            textbox6400Hz.Text = trackbar6400Hz.Value.ToString();
            form1.setParams((byte)TuningFactor.EQFREQ6400, (byte)trackbar6400Hz.Value);
        }

        private void textbox6400Hz_Click(object sender, EventArgs e)
        {
            ((TextBox)sender).SelectAll();
        }

        private void textbox6400Hz_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textbox6400Hz.Text))
            {
                int val = int.Parse(textbox6400Hz.Text);
                if (val > 255)
                {
                    textbox6400Hz.Text = "255";
                    val = 255;
                }
                else if (val < 0)
                {
                    textbox6400Hz.Text = "0";
                    val = 0;
                }
                trackbar6400Hz.Value = val;
            }
        }

        private void textbox6400Hz_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                form1.setParams((byte)TuningFactor.EQFREQ6400, (byte)trackbar6400Hz.Value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }
    }
}
