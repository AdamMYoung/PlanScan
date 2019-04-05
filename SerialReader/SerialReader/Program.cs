using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialReader
{
    class Program
    {
        /// <summary>
        /// Serial port to interface with.
        /// </summary>
        private static SerialPort Port { get; set; }

        /// <summary>
        /// Path of the file to export to.
        /// </summary>
        private static string FilePath { get; set; }

        /// <summary>
        /// Entries to export.
        /// </summary>
        private static IList<PositionalEntry> Entries { get; } = new List<PositionalEntry>();

        static void Main(string[] args)
        {
            Directory.CreateDirectory(@".\Readings");
            Port = new SerialPort("COM8", 9600, Parity.None, 8, StopBits.One);
            Port.DataReceived += Port_DataReceived;

            Port.Open();

            Console.ReadLine();
        }

        /// <summary>
        /// Called on new data from the serial output.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var reading = Port.ReadLine();
            Console.WriteLine(reading);
            if (reading.Length > 2)
            {
                var endChars = reading.Substring(reading.Length - 2, 2);
                if (endChars.Contains("\r"))
                    reading = reading.Substring(0, reading.Length - 1);

                switch (reading)
                {
                    case "TRANSFERSTART":
                        FilePath = @".\Readings\" + DateTime.Now.ToString("yyyy-dd-M--HH-mm-ss") + ".json";
                        break;
                    case "TRANSFEREND":
                        ExportToJson();
                        break;
                    default:
                        HandleReadings(reading);
                        break;
                }
            }
        }

        /// <summary>
        /// Handles the reading from the serial port.
        /// </summary>
        private static void HandleReadings(string reading)
        {
            var parsedString = reading.Split('|');
            if (parsedString.Length == 2)
                StorePosition(parsedString);
            else if (parsedString.Length == 5)
                StoreReading(parsedString);
        }

        /// <summary>
        /// Parses the encoder position from the serial output.
        /// </summary>
        private static void StorePosition(string[] position)
        {
            Entries.Add(new PositionalEntry()
            {
                EncoderXPos = int.Parse(position[0]),
                EncoderYPos = int.Parse(position[1])
            });
        }

        /// <summary>
        /// Parses the reading from the serial output.
        /// </summary>
        private static void StoreReading(string[] reading)
        {
            Entries.Last().Measurements.Add(new DirectionalMeasurement
            {
                Direction = reading[0],
                LeftSensor = int.Parse(reading[1]),
                RightSensor = int.Parse(reading[2]),
                AngleHorizontal = int.Parse(reading[3]),
                AngleVertical = int.Parse(reading[4]),
            });
        }

        /// <summary>
        /// Exports the stored data to JSON.
        /// </summary>
        private static void ExportToJson()
        {
            var json = JsonConvert.SerializeObject(Entries);
            File.WriteAllText(FilePath, json);
            Entries.Clear();
        }
    }
}
