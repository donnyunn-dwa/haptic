using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace a2v_tuner
{
    public partial class WaveformTuner : UserControl
    {
        /// <summary>
        /// Low Frequency Value.
        /// </summary>
        [Description("Low Frequency Value.")]
        public int FreqLow
        {
            get { return freqLow; }
            set { freqLow = value; Invalidate(); }
        }
        int freqLow = 130;

        /// <summary>
        /// Middle Frequency Value.
        /// </summary>
        [Description("Middle Frequency Value.")]
        public int FreqMid
        {
            get { return freqMid; }
            set { freqMid = value; Invalidate(); }
        }
        int freqMid = 165;

        /// <summary>
        /// High Frequency Value.
        /// </summary>
        [Description("High Frequency Value.")]
        public int FreqHigh
        {
            get { return freqHigh; }
            set { freqHigh = value; Invalidate(); }
        }
        int freqHigh = 200;

        /// <summary>
        /// Voltage value at the Low Frequency.
        /// </summary>
        [Description("Voltage value at the Low Frequency.")]
        public byte VoltageLowFreq
        {
            get { return voltageLowFreq; }
            set { voltageLowFreq = value; Invalidate(); }
        }
        byte voltageLowFreq = 75;

        /// <summary>
        /// Voltage value at the Middle Frequency.
        /// </summary>
        [Description("Voltage value at the Middle Frequency.")]
        public byte VoltageMidFreq
        {
            get { return voltageMidFreq; }
            set { voltageMidFreq = value; Invalidate(); }
        }
        byte voltageMidFreq = 37;

        /// <summary>
        /// Voltage value at the High Frequency.
        /// </summary>
        [Description("Voltage value at the High Frequency.")]
        public byte VoltageHighFreq
        {
            get { return voltageHighFreq; }
            set { voltageHighFreq = value; Invalidate(); }
        }
        byte voltageHighFreq = 75;

        public WaveformTuner()
        {
            InitializeComponent();
            //avoid flickering
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            Paint += new PaintEventHandler(WaveformTuner_Paint);
        }
        
        void WaveformTuner_Paint(object sender, PaintEventArgs e)
        {
            PointF[] points = new PointF[64];
            e.Graphics.FillRectangle(Brushes.LightBlue, 0, 0, Width, Height/3);
            e.Graphics.FillRectangle(Brushes.LightGreen, 0, Height/3, Width, Height*2/3);
            e.Graphics.FillRectangle(Brushes.LightPink, 0, Height*2/3, Width, Height);
            
            e.Graphics.DrawLine(Pens.Black, 0, Height / 6, Width, Height / 6 );
            e.Graphics.DrawLine(Pens.Black, 0, Height / 2, Width, Height / 2 );
            e.Graphics.DrawLine(Pens.Black, 0, Height*5/6, Width, Height*5/6 );

            for (int x = 0; x < points.Length; x++)
            {
                points[x] = new PointF((float)((Width - (freqLow - freqLow)) * x / 63), 
                                        (float)((Height/6) * (125 - (voltageLowFreq*Math.Sin(Math.PI * 2 * x / 63)))/125 ));
            }
            e.Graphics.DrawCurve(Pens.Black, points);
            e.Graphics.DrawString(String.Format("{0,3:d}Hz",freqLow), new Font("Courier New", 8f), Brushes.Black, 0, Height/6);
            e.Graphics.DrawString(String.Format("{0,5:f2}V",(float)voltageLowFreq*40/1000), new Font("Courier New", 8f), Brushes.Black, Width/2, 0);

            for (int x = 0; x < points.Length; x++)
            {
                points[x] = new PointF((float)((Width - (freqMid - freqLow)) * x / 63), 
                                        (float)((Height/6)*(125 - (voltageMidFreq*Math.Sin(Math.PI * 2 * x / 63)))/125 + (Height/3)));
            }
            e.Graphics.DrawCurve(Pens.Black, points);
            e.Graphics.DrawString(String.Format("{0,3:d}Hz",freqMid), new Font("Courier New", 8f), Brushes.Black, 0, Height/2);
            e.Graphics.DrawString(String.Format("{0,5:f2}V",(float)voltageMidFreq*40/1000), new Font("Courier New", 8f), Brushes.Black, Width/2, (Height/3));

            for (int x = 0; x < points.Length; x++)
            {
                points[x] = new PointF((float)((Width - (freqHigh - freqLow)) * x / 63), 
                                        (float)((Height/6)*(125 - (voltageHighFreq*Math.Sin(Math.PI * 2 * x / 63)))/125 + (Height*2/3)));
            }
            e.Graphics.DrawCurve(Pens.Black, points);
            e.Graphics.DrawString(String.Format("{0,3:d}Hz",freqHigh), new Font("Courier New", 8f), Brushes.Black, 0, Height*5/6);
            e.Graphics.DrawString(String.Format("{0,5:f2}V",(float)voltageHighFreq*40/1000), new Font("Courier New", 8f), Brushes.Black, Width/2, (Height*2/3));

        }
    }
}
