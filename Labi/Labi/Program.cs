using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Labi
{
    class Program
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("# -------------------- #");
            Console.WriteLine("# ------- LABI ------- #");
            Console.WriteLine("# -------------------- #");
            Console.WriteLine("");
            Console.WriteLine("Commands:");
            Console.WriteLine("\tr              Read lambda");
            Console.WriteLine("\tc <step_size>  Configure step size (micro-meters)");
            Console.WriteLine("\tv              Read step size (micro-meters)");
            Console.WriteLine("\tl <lambda>     Configure step size (micro-meters)");
            Console.WriteLine("\tq              Quit");
            Console.WriteLine("");

            var port = new SerialPort("COM5");
            port.Open();
            port.ReadTimeout = 1000;

            bool run = true;
            while (run)
            {
                try
                {
                    Console.Write(">> ");
                    var command = Console.ReadLine() ?? " ";

                    if (!port.IsOpen)
                        port.Open();

                    switch (command[0])
                    {
                        case 'r':
                            SendReadLambda(port);
                            break;
                        case 'c':
                        {
                            float f;
                            if (!float.TryParse(command.Substring(1, command.Length - 1), out f))
                                Console.WriteLine("Usage: c <float>");
                            else
                                SendConfiguration(port, f);
                        } break;
                        case 'v':
                            SendReadStepSize(port);
                            break;
                        case 'l':
                        {
                            float f;
                            if (!float.TryParse(command.Substring(1, command.Length - 1), out f))
                                Console.WriteLine("Usage: l <float>");
                            else
                                SendLambda(port, f);
                        } break;
                        case 'q':
                            run = false;
                            break;
                    }

                    if (run)
                    {
                        try
                        {
                            var line = port.ReadLine();
                            Console.WriteLine(line);
                        }
                        catch (TimeoutException)
                        {
                            Console.WriteLine("Timeout");
                        }
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }
        }

        private static void SendReadLambda(SerialPort port)
        {
            var buffer = new byte[1];
            buffer[0] = (byte)'r';
            port.Write(buffer, 0, buffer.Length);
        }

        private static void SendReadStepSize(SerialPort port)
        {
            var buffer = new byte[1];
            buffer[0] = (byte)'v';
            port.Write(buffer, 0, buffer.Length);
        }

        private static void SendConfiguration(SerialPort port, float f)
        {
            var buffer = new byte[5];
            buffer[0] = (byte)'c';

            unsafe
            {
                var b = (byte*)&f;
                buffer[1] = b[0];
                buffer[2] = b[1];
                buffer[3] = b[2];
                buffer[4] = b[3];
            }

            port.Write(buffer, 0, buffer.Length);
        }

        private static void SendLambda(SerialPort port, float f)
        {
            var buffer = new byte[5];
            buffer[0] = (byte)'l';

            unsafe
            {
                var b = (byte*)&f;
                buffer[1] = b[0];
                buffer[2] = b[1];
                buffer[3] = b[2];
                buffer[4] = b[3];
            }

            port.Write(buffer, 0, buffer.Length);
        }
    }
}
