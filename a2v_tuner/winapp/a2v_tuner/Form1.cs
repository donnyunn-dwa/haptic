using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace a2v_tuner
{
    public partial class Form1 : Form
    {
        private ControlArea controlArea = null;
        private GraphArea graphArea = null;
        private SoundTuningArea soundTuningArea = null;
        private ActuatorSettingArea actuatorSettingArea = null;
        private Port port = null;
        private Timer playTimer = null;

        private bool bPlay = false;

        public Form1()
        {
            InitializeComponent();

            this.Text = Properties.Resources.TITLE_TEXT;

            this.Load += new EventHandler(this.Form1_Load);
            this.FormClosing += new FormClosingEventHandler(this.Form1_FormClosing);

            this.components = new Container();

            port = new Port();
            port.ReadTimeout = 2000;

            controlArea = new ControlArea(this, port);
            Controls.Add(controlArea);

            graphArea = new GraphArea(this, port);
            Controls.Add(graphArea);

            soundTuningArea = new SoundTuningArea(this, port);
            Controls.Add(soundTuningArea);

            actuatorSettingArea = new ActuatorSettingArea(this, port);
            Controls.Add(actuatorSettingArea);

            InitializeTimers();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            InitializeAreas();
            enableAreas(false);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            port.delay(500);
            if(port.IsOpen)
            {
                port.Close();
            }
        }

        private void InitializeAreas()
        {
            int sWidth = Screen.PrimaryScreen.Bounds.Width *7/ 12;
            int sHeight = Screen.PrimaryScreen.Bounds.Height *7/ 12;
            if (sWidth < 1120) sWidth = 1120;
            if (sHeight < 630) sHeight = 630;
            this.Size = new Size(sWidth, sHeight);
            this.StartPosition = FormStartPosition.WindowsDefaultLocation;

            int aPosX = 0;
            int aPosY = 0;
            controlArea.Location = new Point(aPosX, aPosY);
            controlArea.Size = new Size(sWidth - 16, sHeight / 16);
            aPosY += controlArea.Size.Height;

            graphArea.Location = new Point(aPosX, aPosY);
            graphArea.Size = new Size(sWidth - 16, sHeight *6 / 16);
            aPosY += graphArea.Height;

            soundTuningArea.Location = new Point(aPosX, aPosY);
            soundTuningArea.Size = new Size(sWidth/2, sHeight/2 - 16);
            aPosX += soundTuningArea.Width;

            actuatorSettingArea.Location = new Point(aPosX, aPosY);
            actuatorSettingArea.Size = new Size(sWidth/2 - 16, sHeight/2 - 16);
        }

        private void InitializeTimers()
        {
            playTimer = new Timer(components);
            playTimer.Interval = 10;
            playTimer.Tick += new EventHandler(playTimer_Tick);
        }

        public void enableAreas(bool enable)
        {
            soundTuningArea.Enabled = enable;
            actuatorSettingArea.Enabled = enable;
        }

        public void enableActuatorArea(bool enable)
        {
            actuatorSettingArea.Enabled = enable;
        }

        public void enableBtnConnect(bool enable)
        {
            controlArea.enableBtnConnect(enable);
        }

        public void loadControlValues()
        {
            actuatorSettingArea.loadControlValues();
            soundTuningArea.loadControlValues();
        }

        public void updateGraphThreshold(byte threshold)
        {
            graphArea.updateThreshold(threshold);
        }

        private void playTimer_Tick(object sender, EventArgs e)
        {
            graphArea.updateGraph();
        }

        public void playStart()
        {
            bPlay = true;
            playTimer.Start();
            soundTuningArea.btnPlay.Text = Properties.Resources.BTN_TEXT_STOP;
            
            infoMessage("Monitoring sound value.");
        }

        public void playStop()
        {
            bPlay = false;
            playTimer.Stop();
            soundTuningArea.btnPlay.Text = Properties.Resources.BTN_TEXT_PLAY;

            infoMessage("Stop.");
        }

        public void setParams(byte location, byte value)
        {
            byte[] bytesRes = null;
            int retry = 0;
            do
            {
                do 
                {
                    bytesRes = port.Set_Tuning_Params(location, value);
                    port.delay(1);
                    if(retry++ > 10) {
                        return;
                    }
                } 
                while (bytesRes == null);
            } 
            while (!bytesRes.SequenceEqual(new byte[3] {location, value, 0}));
        }

        public byte getParam(byte location)
        {
            byte[] bytesRes;
            int retry = 0;
            do
            {
                do
                {
                    bytesRes = port.Get_Tuning_Params(location);
                    port.delay(1);
                    if(retry++ > 10) {
                        return 0;
                    }
                }
                while (bytesRes == null);
            }
            while (bytesRes[0] != location);

            return bytesRes[1];
        }

        public void recordParams()
        {
            enableAreas(false);

            byte[] bytesAck = port.Record_Tuning_Params();
            /*
            StringBuilder hex = new StringBuilder(bytesAck.Length * 2);
            foreach(byte b in bytesAck)
                hex.AppendFormat("{0:X2}", b);
            infoMessage(string.Format("Flash write ") + hex.ToString());
            */
            enableAreas(true);
        }

        public int[] getDebugValue()
        {
            byte[] bytesRes = null;
            int retry = 0;
            do
            {
                bytesRes = port.Request_Debug();
                if(retry++ > 10) {
                    return new int[2] {0, 0};
                }
            }
            while (bytesRes == null);
            return new int[2] {BitConverter.ToUInt16(new byte[2] { bytesRes[0], bytesRes[1] }, 0), 
                                    BitConverter.ToUInt16(new byte[2] { bytesRes[2], bytesRes[3] }, 0)};
        }

        public void writeRegister(byte location, byte value)
        {
            byte[] bytesRes = null;
            int retry = 0;
            do
            {
                do 
                {
                    bytesRes = port.Register_Write(location, value);
                    //port.delay(10);
                    if(retry++ > 10) {
                        return;
                    }
                } 
                while (bytesRes == null);
            } 
            while (!bytesRes.SequenceEqual(new byte[3] {location, value, 0}));
        }

        public void recordWaveform()
        {
            enableAreas(false);

            byte[] bytesAck = port.Record_Waveform_MCU();

            enableAreas(true);
        }

        public void VdClamp2VoltMid(byte vdclamp)
        {
            actuatorSettingArea.setVoltageMid(vdclamp);

            actuatorSettingArea.setVoltageLow((int)((double)vdclamp * actuatorSettingArea.getAlpha1()));
            actuatorSettingArea.setVoltageHigh((int)((double)vdclamp * actuatorSettingArea.getAlpha2()));
        }

        public bool isPlay()
        {
            return bPlay;
        }

        public void infoMessage(string msg)
        {
            controlArea.infoMessage(msg);
        }
    }
}
