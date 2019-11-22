using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Drawing;

namespace a2v_tuner
{
    class ActuatorSettingArea : Panel
    {
        private const byte locationFreqLow = 0x14;
        private const byte locationFreqMid = 0x15;
        private const byte locationFreqHigh = 0x16;
        private const byte locationVoltLow = 0x17;
        private const byte locationVoltMid = 0x18;
        private const byte locationVoltHigh = 0x19;

        private double alpha1 = 1.0f;
        private double alpha2 = 1.0f;
        
        private Thread waveformWriteThread { get; set; }
        private Thread waveformRecordThread { get; set; }

        private Form1 form1 = null;
        private Port port1 = null;

        private Button btnWriteRAM = new Button();
        private Button btnWriteFlash = new Button();
        private WaveformTuner waveformTuner = new WaveformTuner();

        private Label labelLowFreqTitle = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleCenter,
        };
        private Label labelMidFreqTitle = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleCenter,
        };
        private Label labelHighFreqTitle = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleCenter,
        };

        private Label labelAlpha1 = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private ComboBox comboBoxVoltLow = new ComboBox() {
            DropDownStyle = ComboBoxStyle.DropDownList, 
        };
        private Label labelUnitVoltLow = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private Label labelFLow = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private TextBox textBoxFreqLow = new TextBox();
        private Label labelHzLow = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private Label labelVdrv = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private Label labelVdrvVariable = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private Label labelFMid = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private TextBox textBoxFreqMid = new TextBox();
        private Label labelHzMid = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private Label labelAlpha2 = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private ComboBox comboBoxVoltHigh = new ComboBox() {
            DropDownStyle = ComboBoxStyle.DropDownList,
        };
        private Label labelUnitVoltHigh = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private Label labelFHigh = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleRight,
        };
        private TextBox textBoxFreqHigh = new TextBox();
        private Label labelHzHigh = new Label() {
            AutoSize = false, TextAlign = ContentAlignment.MiddleLeft,
        };

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // ActuatorSettingArea
            // 
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ActuatorSettingArea_Paint);
            this.ResumeLayout(false);

            Controls.Add(btnWriteRAM);
            btnWriteRAM.Text = "Send Waveform";
            btnWriteRAM.Click += new EventHandler(btnWriteRAM_Click);
            Controls.Add(btnWriteFlash);
            btnWriteFlash.Text = "Save Waveform";
            btnWriteFlash.Click += new EventHandler(btnWriteFlash_Click);

            Controls.Add(waveformTuner);
            waveformTuner.FreqLow = 130;
            waveformTuner.FreqMid = 165;
            waveformTuner.FreqHigh = 200;

            Controls.Add(labelLowFreqTitle);
            labelLowFreqTitle.Font = new Font("Courier New Bold", 12f);
            labelLowFreqTitle.Text = "Low Freq Band";
            labelLowFreqTitle.Click += new EventHandler(labelLowFreqTitle_Click);
            Controls.Add(labelMidFreqTitle);
            labelMidFreqTitle.Font = new Font("Courier New Bold", 12f);
            labelMidFreqTitle.Text = "Mid Freq Band";
            labelMidFreqTitle.Click += new EventHandler(labelMidFreqTitle_Click);
            Controls.Add(labelHighFreqTitle);
            labelHighFreqTitle.Font = new Font("Courier New Bold", 12f);
            labelHighFreqTitle.Text = "High Freq Band";
            labelHighFreqTitle.Click += new EventHandler(labelHighFreqTitle_Click);

            Controls.Add(labelAlpha1);
            labelAlpha1.Font = new Font("Courier New", 12f);
            labelAlpha1.Text = "\u03b1\u2081";
            Controls.Add(comboBoxVoltLow);
            comboBoxVoltLow.Font = new Font("Courier New", 12f);
            for (int i = 0; i < 21; i++)
            {
                comboBoxVoltLow.Items.Add(string.Format("{0:f1}",(double)i*0.1));
            }
            comboBoxVoltLow.Text = "1.0";
            comboBoxVoltLow.SelectedIndexChanged += new EventHandler(comboBoxVoltLow_SelectedIndexChanged);
            Controls.Add(labelUnitVoltLow);
            labelUnitVoltLow.Font = new Font("Courier New", 12f);
            labelUnitVoltLow.Text = "V\u1d05\u0280\u1d20";

            Controls.Add(labelFLow);
            labelFLow.Font = new Font("Courier New", 12f);
            labelFLow.Text = "\u0192\u029f";
            Controls.Add(textBoxFreqLow);
            textBoxFreqLow.Font = new Font("Courier New", 12f);
            textBoxFreqLow.Text = string.Format("{0:d}",waveformTuner.FreqLow);
            textBoxFreqLow.KeyPress += new KeyPressEventHandler(textBoxFreqLow_KeyPress);
            textBoxFreqLow.Leave += new EventHandler(textBoxFreqLow_Leave);
            Controls.Add(labelHzLow);
            labelHzLow.Font = new Font("Courier New", 12f);
            labelHzLow.Text = "Hz";

            Controls.Add(labelVdrv);
            labelVdrv.Font = new Font("Courier New", 12f);
            labelVdrv.Text = "V\u1d05\u0280\u1d20 = ";
            Controls.Add(labelVdrvVariable);
            labelVdrvVariable.Font = new Font("Courier New", 12f);
            labelVdrvVariable.Text = string.Format("{0,2:f2}V",0);

            Controls.Add(labelFMid);
            labelFMid.Font = new Font("Courier New", 12f);
            labelFMid.Text = "\u0192\u1d0d";
            Controls.Add(textBoxFreqMid);
            textBoxFreqMid.Font = new Font("Courier New", 12f);
            textBoxFreqMid.Text = string.Format("{0:d}",waveformTuner.FreqMid);
            textBoxFreqMid.KeyPress += new KeyPressEventHandler(textBoxFreqMid_KeyPress);
            textBoxFreqMid.Leave += new EventHandler(textBoxFreqMid_Leave);
            Controls.Add(labelHzMid);
            labelHzMid.Font = new Font("Courier New", 12f);
            labelHzMid.Text = "Hz";

            Controls.Add(labelAlpha2);
            labelAlpha2.Font = new Font("Courier New", 12f);
            labelAlpha2.Text = "\u03b1\u2082";
            Controls.Add(comboBoxVoltHigh);
            comboBoxVoltHigh.Font = new Font("Courier New", 12f);
            for (int i = 0; i < 21; i++)
            {
                comboBoxVoltHigh.Items.Add(string.Format("{0:f1}",(double)i*0.1));
            }
            comboBoxVoltHigh.Text = "1.0";
            comboBoxVoltHigh.SelectedIndexChanged += new EventHandler(comboBoxVoltHigh_SelectedIndexChanged);
            Controls.Add(labelUnitVoltHigh);
            labelUnitVoltHigh.Font = new Font("Courier New", 12f);
            labelUnitVoltHigh.Text = "V\u1d05\u0280\u1d20";

            Controls.Add(labelFHigh);
            labelFHigh.Font = new Font("Courier New", 12f);
            labelFHigh.Text = "\u0192\u029c";
            Controls.Add(textBoxFreqHigh);
            textBoxFreqHigh.Font = new Font("Courier New", 12f);
            textBoxFreqHigh.Text = string.Format("{0:d}",waveformTuner.FreqHigh);
            textBoxFreqHigh.KeyPress += new KeyPressEventHandler(textBoxFreqHigh_KeyPress);
            textBoxFreqHigh.Leave += new EventHandler(textBoxFreqHigh_Leave);
            Controls.Add(labelHzHigh);
            labelHzHigh.Font = new Font("Courier New", 12f);
            labelHzHigh.Text = "Hz";
        }

        public ActuatorSettingArea(Form1 form, Port port)
        {
            this.form1 = form;
            this.port1 = port;

            InitializeComponent();

            this.BackColor = SystemColors.Window;

            //setVoltageLow((int)(waveformTuner.VoltageMidFreq * double.Parse(comboBoxVoltLow.Text)));
            //setVoltageHigh((int)(waveformTuner.VoltageMidFreq * double.Parse(comboBoxVoltHigh.Text)));
        }

        private void ActuatorSettingArea_Paint(object sender, PaintEventArgs e)
        {
            InitializeControls();
        }

        private void InitializeControls()
        {
            int cPosX = 8;
            int cPosY = 0;

            btnWriteRAM.Location = new Point(cPosX, cPosY);
            btnWriteRAM.Size = new Size(this.Width / 6, this.Height / 8);
            cPosX += btnWriteRAM.Width;
            btnWriteFlash.Location = new Point(cPosX, cPosY);
            btnWriteFlash.Size = new Size(this.Width / 6, this.Height / 8);
            cPosY += btnWriteFlash.Height;

            cPosX = 8;
            waveformTuner.Location = new Point(cPosX, cPosY);
            waveformTuner.Size = new Size(this.Width / 3, this.Height*3/ 4);
            cPosX += waveformTuner.Width;

            labelLowFreqTitle.Location = new Point(cPosX, cPosY);
            labelLowFreqTitle.Size = new Size(this.Width / 4, waveformTuner.Height / 3);
            cPosY += labelLowFreqTitle.Height;
            labelMidFreqTitle.Location = new Point(cPosX, cPosY);
            labelMidFreqTitle.Size = new Size(this.Width / 4, waveformTuner.Height / 3);
            cPosY += labelMidFreqTitle.Height;
            labelHighFreqTitle.Location = new Point(cPosX, cPosY);
            labelHighFreqTitle.Size = new Size(this.Width / 4, waveformTuner.Height / 3);
            
            cPosX += labelLowFreqTitle.Width;
            cPosY = btnWriteFlash.Height;
            labelAlpha1.Location = new Point(cPosX, cPosY);
            labelAlpha1.Size = new Size(this.Width / 12, waveformTuner.Height / 6);
            cPosY += labelAlpha1.Height;
            labelFLow.Location = new Point(cPosX, cPosY);
            labelFLow.Size = new Size(this.Width / 12, waveformTuner.Height / 6);
            cPosY += labelFLow.Height;
            labelVdrv.Location = new Point(cPosX, cPosY);
            labelVdrv.Size = new Size(this.Width / 12 + 36, waveformTuner.Height / 6);
            cPosY += labelVdrv.Height;
            labelFMid.Location = new Point(cPosX, cPosY);
            labelFMid.Size = new Size(this.Width / 12, waveformTuner.Height / 6);
            cPosY += labelFMid.Height;
            labelAlpha2.Location = new Point(cPosX, cPosY);
            labelAlpha2.Size = new Size(this.Width / 12, waveformTuner.Height / 6);
            cPosY += labelAlpha2.Height;
            labelFHigh.Location = new Point(cPosX, cPosY);
            labelFHigh.Size = new Size(this.Width / 12, waveformTuner.Height / 6);

            cPosX += labelAlpha1.Width;
            cPosY = btnWriteFlash.Height+8;
            comboBoxVoltLow.Location = new Point(cPosX, cPosY);
            comboBoxVoltLow.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += comboBoxVoltLow.Height+8;
            textBoxFreqLow.Location = new Point(cPosX, cPosY);
            textBoxFreqLow.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += textBoxFreqLow.Height+8;
            labelVdrvVariable.Location = new Point(cPosX + 36, cPosY);
            labelVdrvVariable.Size = new Size(this.Width / 6, waveformTuner.Height / 6);
            cPosY += labelVdrvVariable.Height+8;
            textBoxFreqMid.Location = new Point(cPosX, cPosY);
            textBoxFreqMid.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += textBoxFreqMid.Height+8;
            comboBoxVoltHigh.Location = new Point(cPosX, cPosY);
            comboBoxVoltHigh.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += comboBoxVoltHigh.Height+8;
            textBoxFreqHigh.Location = new Point(cPosX, cPosY);
            textBoxFreqHigh.Size = new Size(this.Width / 9, waveformTuner.Height / 6);

            cPosX += textBoxFreqHigh.Width;
            cPosY = btnWriteFlash.Height;
            labelUnitVoltLow.Location = new Point(cPosX, cPosY);
            labelUnitVoltLow.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += labelUnitVoltLow.Height;
            labelHzLow.Location = new Point(cPosX, cPosY);
            labelHzLow.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += labelVdrvVariable.Height;
            cPosY += labelHzLow.Height;
            labelHzMid.Location = new Point(cPosX, cPosY);
            labelHzMid.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += labelHzMid.Height;
            labelUnitVoltHigh.Location = new Point(cPosX, cPosY);
            labelUnitVoltHigh.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
            cPosY += labelUnitVoltHigh.Height;
            labelHzHigh.Location = new Point(cPosX, cPosY);
            labelHzHigh.Size = new Size(this.Width / 9, waveformTuner.Height / 6);
        }

        public void loadControlValues()
        {
            byte vlow, vmid, vhigh;

            setFrequencyLow(form1.getParam(locationFreqLow));
            setFrequencyMid(form1.getParam(locationFreqMid));
            setFrequencyHigh(form1.getParam(locationFreqHigh));

            textBoxFreqLow.Text = string.Format("{0:d}",waveformTuner.FreqLow);
            textBoxFreqMid.Text = string.Format("{0:d}",waveformTuner.FreqMid);
            textBoxFreqHigh.Text = string.Format("{0:d}",waveformTuner.FreqHigh);

            vlow = form1.getParam(locationVoltLow);
            vmid = form1.getParam(locationVoltMid);
            vhigh = form1.getParam(locationVoltHigh);

            if (vlow == 0) vlow++;
            waveformTuner.VoltageLowFreq = vlow;
            if (vmid == 0) vmid++;
            waveformTuner.VoltageMidFreq = vmid;
            if (vhigh == 0) vhigh++;
            waveformTuner.VoltageHighFreq = vhigh;

            alpha1 = (double)vlow / (double)vmid;
            comboBoxVoltLow.SelectedIndex = (int)(alpha1*10.0f);
            alpha2 = (double)vhigh / (double)vmid;
            comboBoxVoltHigh.SelectedIndex = (int)(alpha2*10.0f);
        }

        public double getAlpha1()
        {
            return alpha1;
        }

        public double getAlpha2()
        {
            return alpha2;
        }

        public void setVoltageLow(int voltage)
        {
            waveformTuner.VoltageLowFreq = (byte)(voltage < 256 ? voltage : 255);
            form1.setParams((byte)locationVoltLow, waveformTuner.VoltageLowFreq);
        }

        public void setVoltageMid(int voltage)
        {
            labelVdrvVariable.Text = string.Format("{0,2:f2}V",(double)voltage*0.04);
            waveformTuner.VoltageMidFreq = (byte)(voltage < 256 ? voltage : 255);
            form1.setParams((byte)locationVoltMid, waveformTuner.VoltageMidFreq);
        }

        public void setVoltageHigh(int voltage)
        {
            waveformTuner.VoltageHighFreq = (byte)(voltage < 256 ? voltage : 255);
            form1.setParams((byte)locationVoltHigh, waveformTuner.VoltageHighFreq);
        }

        public void setFrequencyLow(int frequency)
        {
            byte FreqLow = (byte)(frequency < 256 ? frequency : 255);
            waveformTuner.FreqLow = FreqLow;
        }

        public void setFrequencyMid(int frequency)
        {
            waveformTuner.FreqMid = (byte)(frequency < 256 ? frequency : 255);
        }

        public void setFrequencyHigh(int frequency)
        {
            waveformTuner.FreqHigh = (byte)(frequency < 256 ? frequency : 255);
        }

        private void btnWriteRAM_Click(object sender, EventArgs e)
        {
            waveformWriteThread = new Thread(new ThreadStart(this.threadWaveformWrite));
            waveformWriteThread.Start();
        }

        private void threadWaveformWrite()
        {
            this.Invoke(new Action(delegate()
            {
                form1.infoMessage(Properties.Resources.NOTI_UPDATING_WAVEFORM);
                form1.Enabled = false;

                form1.recordParams();
                this.waveformWrite();

                form1.Enabled = true;
                form1.infoMessage(Properties.Resources.NOTI_UPDATE_COMPLETE);
            }));
        }

        private void waveformWrite()
        {
            short waveAddr = 0x027C;
            short waveLength;
            short headerAddr = 0x0001;
            short headerLength = 5;
            byte pcm;
            
            // info
            form1.setParams((byte)locationVoltLow, waveformTuner.VoltageLowFreq);
            form1.setParams(locationFreqLow, (byte)int.Parse(textBoxFreqLow.Text));
            form1.setParams((byte)locationVoltMid, waveformTuner.VoltageMidFreq);
            form1.setParams(locationFreqMid, (byte)int.Parse(textBoxFreqMid.Text));
            form1.setParams((byte)locationVoltHigh, waveformTuner.VoltageHighFreq);
            form1.setParams(locationFreqHigh, (byte)int.Parse(textBoxFreqHigh.Text));

            // low band
            waveLength = (short)(48000 / waveformTuner.FreqLow);
            form1.writeRegister(0x46, (byte)(waveAddr >> 8));
            form1.writeRegister(0x47, (byte)(waveAddr & 0xff));
            for (int i = 0; i < waveLength; i++)
            {
                pcm = (byte)(0x7f*Math.Sin((double)(i * (2 * Math.PI) / waveLength)));
                form1.writeRegister(0x48, pcm);
                form1.infoMessage(Properties.Resources.NOTI_UPDATING_WAVEFORM + string.Format(" {0:d}%",(int)((double)i/waveLength*33)));
            }
            form1.writeRegister(0x46, (byte)(headerAddr >> 8));
            form1.writeRegister(0x47, (byte)(headerAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveAddr >> 8));
            form1.writeRegister(0x48, (byte)(waveAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveLength >> 8));
            form1.writeRegister(0x48, (byte)(waveLength & 0xff));
            form1.writeRegister(0x48, waveformTuner.VoltageLowFreq);
            waveAddr += waveLength;

            // middle band
            headerAddr += headerLength;
            waveLength = (short)(48000 / waveformTuner.FreqMid);
            form1.writeRegister(0x46, (byte)(waveAddr >> 8));
            form1.writeRegister(0x47, (byte)(waveAddr & 0xff));
            for (int i = 0; i < waveLength; i++)
            {
                pcm = (byte)(0x7f*Math.Sin((double)(i * (2 * Math.PI) / waveLength)));
                form1.writeRegister(0x48, pcm);
                form1.infoMessage(Properties.Resources.NOTI_UPDATING_WAVEFORM + string.Format(" {0:d}%",33+(int)((double)i/waveLength*33)));
            }
            form1.writeRegister(0x46, (byte)(headerAddr >> 8));
            form1.writeRegister(0x47, (byte)(headerAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveAddr >> 8));
            form1.writeRegister(0x48, (byte)(waveAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveLength >> 8));
            form1.writeRegister(0x48, (byte)(waveLength & 0xff));
            form1.writeRegister(0x48, waveformTuner.VoltageMidFreq);
            waveAddr += waveLength;

            // high band
            headerAddr += headerLength;
            waveLength = (short)(48000 / waveformTuner.FreqHigh);
            form1.writeRegister(0x46, (byte)(waveAddr >> 8));
            form1.writeRegister(0x47, (byte)(waveAddr & 0xff));
            for (int i = 0; i < waveLength; i++)
            {
                pcm = (byte)(0x7f*Math.Sin((double)(i * (2 * Math.PI) / waveLength)));
                form1.writeRegister(0x48, pcm);
                form1.infoMessage(Properties.Resources.NOTI_UPDATING_WAVEFORM + string.Format(" {0:d}%",67+(int)((double)i/waveLength*33)));
            }
            form1.writeRegister(0x46, (byte)(headerAddr >> 8));
            form1.writeRegister(0x47, (byte)(headerAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveAddr >> 8));
            form1.writeRegister(0x48, (byte)(waveAddr & 0xff));
            form1.writeRegister(0x48, (byte)(waveLength >> 8));
            form1.writeRegister(0x48, (byte)(waveLength & 0xff));
            form1.writeRegister(0x48, waveformTuner.VoltageHighFreq);
            waveAddr += waveLength;
        }

        private void btnWriteFlash_Click(object sender, EventArgs e)
        {
            waveformRecordThread = new Thread(new ThreadStart(this.threadWaveformRecord));
            waveformRecordThread.Start();
        }
        
        private void threadWaveformRecord()
        {
            this.Invoke(new Action(delegate()
            {
                form1.infoMessage(Properties.Resources.NOTI_SAVING_WAVEFORM);
                form1.Enabled = false;

                form1.recordParams();
                form1.recordWaveform();

                form1.Enabled = true;
                form1.infoMessage(Properties.Resources.NOTI_SAVE_COMPLETE);
            }));
        }

        private void labelLowFreqTitle_Click(object sender, EventArgs e)
        {
            form1.writeRegister(0x0F, 1);
            form1.writeRegister(0x0C, 1);
        }

        private void labelMidFreqTitle_Click(object sender, EventArgs e)
        {
            form1.writeRegister(0x0F, 2);
            form1.writeRegister(0x0C, 1);
        }

        private void labelHighFreqTitle_Click(object sender, EventArgs e)
        {
            form1.writeRegister(0x0F, 3);
            form1.writeRegister(0x0C, 1);
        }

        private void comboBoxVoltLow_SelectedIndexChanged(object sender, EventArgs e)
        {
            alpha1 = double.Parse(comboBoxVoltLow.Text);
            setVoltageLow((int)((double)(waveformTuner.VoltageMidFreq) * alpha1));
        }

        private void comboBoxVoltHigh_SelectedIndexChanged(object sender, EventArgs e)
        {
            alpha2 = double.Parse(comboBoxVoltHigh.Text);
            setVoltageHigh((int)((double)(waveformTuner.VoltageMidFreq) * alpha2));
        }

        private void textBoxFreqLow_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            int value;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                value = int.Parse(textBoxFreqLow.Text);
                setFrequencyLow(value);
                form1.setParams(locationFreqLow, (byte)value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void textBoxFreqLow_Leave(object sender, EventArgs e)
        {
            int value;
            value = int.Parse(textBoxFreqLow.Text);
            setFrequencyLow(value);
            form1.setParams(locationFreqLow, (byte)value);
        }

        private void textBoxFreqMid_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            int value;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                value = int.Parse(textBoxFreqMid.Text);
                setFrequencyMid(value);
                form1.setParams(locationFreqMid, (byte)value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void textBoxFreqMid_Leave(object sender, EventArgs e)
        {
            int value;
            value = int.Parse(textBoxFreqMid.Text);
            setFrequencyMid(value);
            form1.setParams(locationFreqMid, (byte)value);
        }

        private void textBoxFreqHigh_KeyPress(object sender, KeyPressEventArgs e)
        {
            int keycode = (int)e.KeyChar;
            int value;
            if (keycode == (int)Keys.Enter || keycode == (int)Keys.Tab)
            {
                value = int.Parse(textBoxFreqHigh.Text);
                setFrequencyHigh(value);
                form1.setParams(locationFreqHigh, (byte)value);
            }
            else if ((keycode < '0' || keycode > '9') && keycode != 8)
            {
                e.Handled = true;
            }
        }

        private void textBoxFreqHigh_Leave(object sender, EventArgs e)
        {
            int value;
            value = int.Parse(textBoxFreqHigh.Text);
            setFrequencyHigh(value);
            form1.setParams(locationFreqHigh, (byte)value);
        }
    }
}
