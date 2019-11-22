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
    public partial class MultiTrackbar : UserControl
    {
        /// <summary>
        /// Minimum value of the slider.
        /// </summary>
        [Description("Minimum value of the slider.")]
        public int Min
        {
            get { return min; }
            set { min = value; Invalidate(); }
        }
        int min = 0;
        /// <summary>
        /// Maximum value of the slider.
        /// </summary>
        [Description("Maximum value of the slider.")]
        public int Max
        {
            get { return max; }
            set { max = value; Invalidate(); }
        }
        int max = 100;
        /// <summary>
        /// Minimum value of the selection range.
        /// </summary>
        [Description("Minimum value of the selection range.")]
        public int SelectedMin
        {
            get { return selectedMin; }
            set
            {
                selectedMin = value;
                if (SelectionChanged != null)
                    SelectionChanged(this, null);
                Invalidate();
            }
        }
        int selectedMin = 0;
        /// <summary>
        /// Maximum value of the selection range.
        /// </summary>
        [Description("Maximum value of the selection range.")]
        public int SelectedMax
        {
            get { return selectedMax; }
            set
            {
                selectedMax = value;
                if (SelectionChanged != null)
                    SelectionChanged(this, null);
                Invalidate();
            }
        }
        int selectedMax = 100;
        /// <summary>
        /// Current value.
        /// </summary>
        [Description("Current value.")]
        public int Value
        {
            get { return value; }
            set
            {
                this.value = value;
                if (ValueChanged != null)
                    ValueChanged(this, null);
                Invalidate();
            }
        }
        int value = 50;
        /// <summary>
        /// Fired when SelectedMin or SelectedMax changes.
        /// </summary>
        [Description("Fired when SelectedMin or SelectedMax changes.")]
        public event EventHandler SelectionChanged;
        /// <summary>
        /// Fired when Value changes.
        /// </summary>
        [Description("Fired when Value changes.")]
        public event EventHandler ValueChanged;

        public MultiTrackbar()
        {
            InitializeComponent();

            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            Paint += new PaintEventHandler(SelectionRangeSlider_Paint);
            MouseDown += new MouseEventHandler(SelectionRangeSlider_MouseDown);
            MouseMove += new MouseEventHandler(SelectionRangeSlider_MouseMove);
        }

        void SelectionRangeSlider_Paint(object sender, PaintEventArgs e)
        {
            //paint background in white
            e.Graphics.FillRectangle(Brushes.White, ClientRectangle);
            //paint selection range in blue
            Rectangle selectionRect = new Rectangle(
                (selectedMin - Min) * Width / (Max - Min),
                0,
                (selectedMax - selectedMin) * Width / (Max - Min),
                Height);
            e.Graphics.FillRectangle(Brushes.LightBlue, 0, 0, SelectedMin * Width / (Max - Min), Height);
            e.Graphics.FillRectangle(Brushes.LightGreen, SelectedMin * Width / (Max - Min), 0, (Value - SelectedMin) * Width / (Max - Min), Height);
            e.Graphics.FillRectangle(Brushes.LightPink, Value * Width / (Max - Min), 0, (selectedMax - Value) * Width / (Max - Min), Height);

            e.Graphics.DrawRectangle(Pens.Black, selectionRect);
            //draw a black frame around our control
            e.Graphics.DrawRectangle(Pens.Black, 0, 0, Width - 1, Height - 1);
            //draw a simple vertical line at the Value position
            e.Graphics.DrawLine(Pens.Black,
                (Value - Min) * Width / (Max - Min), 0,
                (Value - Min) * Width / (Max - Min), Height);

        }

        void SelectionRangeSlider_MouseDown(object sender, MouseEventArgs e)
        {
            //check where the user clicked so we can decide which thumb to move
            int pointedValue = Min + e.X * (Max - Min) / Width;
            int distValue = Math.Abs(pointedValue - Value);
            int distMin = Math.Abs(pointedValue - SelectedMin);
            int distMax = Math.Abs(pointedValue - SelectedMax);
            int minDist = Math.Min(distValue, Math.Min(distMin, distMax));
            if (minDist == distValue)
                movingMode = MovingMode.MovingValue;
            else if (minDist == distMin)
                movingMode = MovingMode.MovingMin;
            else
                movingMode = MovingMode.MovingMax;
            //call this to refreh the position of the selected thumb
            SelectionRangeSlider_MouseMove(sender, e);
        }

        void SelectionRangeSlider_MouseMove(object sender, MouseEventArgs e)
        {
            //if the left button is pushed, move the selected thumb
            if (e.Button != MouseButtons.Left)
                return;
            int pointedValue = Min + e.X * (Max - Min) / Width;
            if (movingMode == MovingMode.MovingValue)
            {
                if (pointedValue <= selectedMin)
                {
                    pointedValue = selectedMin + 1;
                }
                else if (pointedValue >= selectedMax)
                {
                    pointedValue = selectedMax - 1;
                }
                Value = pointedValue;
            }
            else if (movingMode == MovingMode.MovingMin)
            {
                if (pointedValue >= Value)
                {
                    pointedValue = Value - 1;
                }
                else if (pointedValue <= Min)
                {
                    pointedValue = Min + 1;
                }
                SelectedMin = pointedValue;
            }
            else if (movingMode == MovingMode.MovingMax)
            {
                if (pointedValue <= Value)
                {
                    pointedValue = Value + 1;
                }
                else if (pointedValue > Max)
                {
                    pointedValue = Max;
                }
                SelectedMax = pointedValue;
            }
        }

        /// <summary>
        /// To know which thumb is moving
        /// </summary>
        enum MovingMode { MovingValue, MovingMin, MovingMax }
        MovingMode movingMode;
        public int getMovingMode()
        {
            int ret = 0;
            switch (movingMode)
            {
                case MovingMode.MovingMin:
                    ret = 1;
                    break;
                case MovingMode.MovingValue:
                    ret = 2;
                    break;
                case MovingMode.MovingMax:
                    ret = 3;
                    break;
                default:
                    ret = 0;
                    break;
            }
            return ret;
        }
    }
}
