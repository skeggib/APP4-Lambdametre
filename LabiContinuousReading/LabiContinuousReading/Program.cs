using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LabiContinuousReading
{
    class Program
    {
        static void Main(string[] args)
        {
            var port = new SerialPort("COM5");
            port.Open();

            while (true)
            {
                port.Write(new byte[1] { (byte)'r' }, 0, 1);
                Console.WriteLine(port.ReadLine());
            }
        }
    }
}
