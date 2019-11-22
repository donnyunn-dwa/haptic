using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;

namespace a2v_tuner
{
    class GraphArea : Panel
    {
        private Form1 form1 = null;
        Port port1 = null;
        ChartArea chartArea = new ChartArea();
        Legend legend = new Legend();
        Series series = new Series();
        private Chart chart = new Chart();

        public Thread playThread { get; set; }

        private const int dbgXLen = 256;
        private int dbgYLen = 260;

        private int[] dbgData1 = new int[dbgXLen];
        private int[] dbgData2 = new int[dbgXLen];

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // GraphArea
            // 
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.GraphArea_Paint);
            this.ResumeLayout(false);
        }

        public GraphArea(Form1 form, Port port)
        {
            this.form1 = form;
            this.port1 = port;

            InitializeComponent();

            this.BackColor = SystemColors.Window;

            InitializeChart();
            Controls.Add(chart);
        }

        private void GraphArea_Paint(object sender, PaintEventArgs e)
        {
            chart.Location = new Point(-32, 0);
            chart.Size = new Size(this.Width + 32, this.Height - 8);
        }

        private void InitializeChart()
        {
            chart.ChartAreas.Add(chartArea);
            legend.Name = "Legend";
            chart.Legends.Add(legend);
            chart.Name = "chart";
            chartArea.Name = "ChartArea";
            series.ChartArea = "ChartArea";
            series.Legend = "Legend";
            series.Name = "Series";
            chart.Series.Add(series);
            chart.Text = "chart";

            chart.ChartAreas.Clear();
            chart.Series.Clear();

            chart.ChartAreas.Add("Draw");
            chart.ChartAreas["Draw"].BackColor = Color.LightYellow;
            chart.ChartAreas["Draw"].BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;

            chart.ChartAreas["Draw"].AxisX.Minimum = 0;
            chart.ChartAreas["Draw"].AxisX.Maximum = dbgXLen;
            chart.ChartAreas["Draw"].AxisX.Interval = 16;
            chart.ChartAreas["Draw"].AxisX.MajorGrid.LineColor = Color.LightGray;
            chart.ChartAreas["Draw"].AxisX.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dash;
            chart.ChartAreas["Draw"].AxisX.LabelStyle.Enabled = false;

            chart.ChartAreas["Draw"].AxisY.Minimum = -10;
            chart.ChartAreas["Draw"].AxisY.Maximum = dbgYLen;
            chart.ChartAreas["Draw"].AxisY.Interval = 20;
            chart.ChartAreas["Draw"].AxisY.MajorGrid.LineColor = Color.LightGray;
            chart.ChartAreas["Draw"].AxisY.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dash;
            chart.ChartAreas["Draw"].AxisY.LabelStyle.Enabled = true;
            chart.ChartAreas["Draw"].AxisY.LabelAutoFitStyle = LabelAutoFitStyles.None;
            chart.ChartAreas["Draw"].AxisY.LabelStyle.Font = new System.Drawing.Font("Arial", 10, System.Drawing.FontStyle.Regular);

            chart.Legends["Legend"].Position.Auto = false;
            chart.Legends["Legend"].Position = new ElementPosition(85, 5, 12, 32);
            chart.Legends["Legend"].BackColor = Color.Transparent;

            chart.Series.Add("data1");
            chart.Series["data1"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart.Series["data1"].Color = Color.Black;
            chart.Series["data1"].BorderWidth = 2;
            chart.Series["data1"].LegendText = "Sound_L";
            
            chart.Series.Add("data2");
            chart.Series["data2"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart.Series["data2"].Color = Color.Red;
            chart.Series["data2"].BorderWidth = 2;
            chart.Series["data2"].LegendText = "Sound_R";

            for (int x = 0; x < dbgXLen; x++)
            {
                chart.Series["data1"].Points.AddXY(x, dbgData1[x]);
                chart.Series["data2"].Points.AddXY(x, dbgData2[x]);
            }

            chart.Series.Add("Thre");
            chart.Series["Thre"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart.Series["Thre"].Color = Color.Green;
            chart.Series["Thre"].BorderWidth = 2;
            chart.Series["Thre"].LegendText = "Threshold";

            chart_threshold_update(0);
        }
        private void chart_threshold_update(int threshold)
        {
            chart.Series["Thre"].Points.Clear();
            chart.Series["Thre"].Points.AddXY(0, threshold);
            chart.Series["Thre"].Points.AddXY(dbgXLen, threshold);
        }

        private void chart_data_update(int data1, int data2)
        {
            int lastX = dbgXLen - 1;
            for (int x = 0; x < lastX; x++)
            {
                dbgData1[x] = dbgData1[x+1];
                dbgData2[x] = dbgData2[x+1];
            }
            dbgData1[lastX] = data1;
            dbgData2[lastX] = data2;

            chart.Series["data1"].Points.Clear();
            chart.Series["data2"].Points.Clear();
            for (int x = 0; x < dbgXLen; x++)
            {
                chart.Series["data1"].Points.AddXY(x, dbgData1[x]);
                chart.Series["data2"].Points.AddXY(x, dbgData2[x]);
            }
        }

        public void updateGraph()
        {
            if (playThread != null && playThread.IsAlive) return;
            playThread = new Thread(new ThreadStart(threadPlay));
            playThread.Start();
        }

        private void threadPlay()
        {
            byte[] rx = new byte[7];
            int[] data = new int[2];
            this.Invoke(new Action(delegate()
            {
                data = form1.getDebugValue();
                this.chart_data_update(data[0], data[1]);
            }));
        }

        public void updateThreshold(byte threshold)
        {
            chart_threshold_update(threshold);
        }
    }
}
