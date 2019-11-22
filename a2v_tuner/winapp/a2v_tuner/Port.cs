using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Management;
using System.Diagnostics;
using System.Windows.Forms;

namespace a2v_tuner
{
    class Port : SerialPort
    {
        private const ushort cmdPrefix = 0x7914;

        public int Connect(int baudrate)
        {
            int ret = -1;
            string umft234xdCOMDevice = FindPortByDescription("USB Serial Port");

            if (umft234xdCOMDevice != null) 
            {
                PortName = umft234xdCOMDevice;
                BaudRate = baudrate;
                DataBits = 8;
                Parity = Parity.None;
                Handshake = Handshake.None;
                StopBits = StopBits.One;

                try
                {
                    Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return ret;
                }

                ret = 0;
            }

            return ret;
        }

        public int Check_Connection()
        {
            int ret = -1;
            byte[] rx = null;
            string strRx;

            ret = SendCommand("$00000000#");
            if(ret != 0)
            {
                return ret;
            }

            rx = DepacketResponse(cmdPrefix, 0x0000);
            if(rx != null)
            {
                strRx = Encoding.Default.GetString(rx);
                if(strRx.Equals("DW7914\0"))
                {
                    ret = 0;
                }
            }
            
            return ret;
        }

        public byte[] Set_Tuning_Params(byte loc, byte val)
        {
            if(SendCommand(string.Format("$0001{0:X2}{1:X2}#", loc, val)) != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0001);
        }

        public byte[] Get_Tuning_Params(byte loc)
        {
            if(SendCommand(string.Format("$0002{0:X2}00#", loc)) != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0002);
        }

        public byte[] Request_Debug()
        {
            if(SendCommand("$00030E06#") != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0003);
        }

        public byte[] Record_Tuning_Params()
        {
            if(SendCommand("$00040020#") != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0004);
        }

        public byte[] Register_Write(byte loc, byte val)
        {
            if(SendCommand(string.Format("$0005{0:X2}{1:X2}#", loc, val)) != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0005);
        }

        public byte[] Record_Waveform_MCU()
        {
            if(SendCommand("$00060000#") != 0)
            {
                return null;
            }
            return DepacketResponse(cmdPrefix, 0x0006);
        }

        private byte[] DepacketResponse(ushort hdrCode, ushort cmdCode)
        {
            byte[] ret = null;
            byte[] rx = null;
            ushort cmdLen;

            try
            {
                rx = new byte[2];
                rx[0] = (byte)ReadByte();
                rx[1] = (byte)ReadByte();
                if(hdrCode == BitConverter.ToUInt16(new byte[2] { rx[0], rx[1] }, 0))
                {
                    rx = new byte[2];
                    rx[0] = (byte)ReadByte();
                    rx[1] = (byte)ReadByte();
                    if(cmdCode == BitConverter.ToUInt16(new byte[2] { rx[0], rx[1] }, 0))
                    {
                        rx = new byte[2];
                        rx[0] = (byte)ReadByte();
                        rx[1] = (byte)ReadByte();
                        cmdLen = BitConverter.ToUInt16(new byte[2] { rx[0], rx[1] }, 0);
                        
                        ret = new byte[cmdLen];
                        Read(ret, 0, cmdLen);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return null;
            }

            return ret;
        }

        private int SendCommand(string cmd)
        {
            int ret = -1;
            byte[] tx = null;

            if(!IsOpen) 
            {
                return ret;
            }

            tx = new byte[cmd.Length];
            tx = Encoding.UTF8.GetBytes(cmd);
            try
            {
                Write(tx, 0, 10);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return ret;
            }
            ret = 0;

            return ret;
        }

        static string[] FindAllPorts()
        {
            List<string> ports = new List<string>();

            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Caption"].ToString().Contains("(COM"))
                    {
                        string comName = ParseCOMName(obj);
                        if (comName != null)
                        {
                            ports.Add(comName);
                        }
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e);
                }

            }

            return ports.ToArray();
        }

        static string FindPortByDescription(string description)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Description"].ToString().ToLower().Equals(description.ToLower()))
                    {
                        string comName = ParseCOMName(obj);
                        if (comName != null)
                        {
                            return comName;
                        }
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e);
                }
            }

            return null;
        }

        static ManagementObject[] FindPorts()
        {
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity");
                List<ManagementObject> objects = new List<ManagementObject>();

                foreach (ManagementObject obj in searcher.Get())
                {
                    objects.Add(obj);
                }

                return objects.ToArray();
            }
            catch (Exception e)
            {
                Debug.WriteLine(e);
                return new ManagementObject[] { };
            }
        }

        static string ParseCOMName(ManagementObject obj)
        {
            string name = obj["Name"].ToString();
            int startIndex = name.LastIndexOf("(");
            int endIndex = name.LastIndexOf(")");

            if (startIndex != -1 && endIndex != -1)
            {
                name = name.Substring(startIndex + 1, endIndex - startIndex - 1);

                return name;
            }
            return null;
        }
        private static DateTime Delay(int MS)
        {
            DateTime ThisMoment = DateTime.Now;
            TimeSpan duration = new TimeSpan(0, 0, 0, 0, MS);
            DateTime AfterWards = ThisMoment.Add(duration);
            while (AfterWards >= ThisMoment)            {
                System.Windows.Forms.Application.DoEvents();
                ThisMoment = DateTime.Now;
            }
            return DateTime.Now;
        }

        public void delay(int ms)
        {
            Delay(ms);
        }
    }
}
