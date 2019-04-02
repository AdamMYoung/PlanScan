using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialReader
{
    class Program
    {
        private static SerialPort port;
        static void Main(string[] args)
        {
            port = new SerialPort("COM8", 9600, Parity.None, 8, StopBits.One);
            port.DataReceived += Port_DataReceived;

            port.Open();

            Console.ReadLine();
        }

        private static void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Console.WriteLine(port.ReadExisting());
        }
    }
}
