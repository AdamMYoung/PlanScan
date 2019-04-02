using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialReader
{
    class PositionalEntry
    {
        public int EncoderXPos { get; set; }
        public int EncoderYPos { get; set; }
        public IList<DirectionalMeasurement> Measurements = new List<DirectionalMeasurement>();
    }
}
