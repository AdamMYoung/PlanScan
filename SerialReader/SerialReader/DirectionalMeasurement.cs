using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialReader
{
    class DirectionalMeasurement
    {
        public String Direction { get; set; }
        public int LeftSensor { get; set; }
        public int RightSensor { get; set; }
        public int AngleHorizontal { get; set; }
        public int AngleVertical { get; set; }
    }
}
