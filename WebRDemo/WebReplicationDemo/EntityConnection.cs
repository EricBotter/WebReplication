using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebReplicationDemo
{
    class EntityConnection
    {
        public NetworkEntity client;
        public NetworkEntity server;
        public List<Packet> packets = new List<Packet>();

        public EntityConnection(NetworkEntity client, NetworkEntity server)
        {
            this.client = client;
            this.server = server;
        }

        public void addPacket(Packet p)
        {
            packets.Add(p);
        }
    }

    class Packet
    {
        enum PacketType
        {
            RECEIVED,
            REQUESTED,
            COMPLETED,
            PROCESSED,
            TORESOLVE,
            RESOLVED
        }

        PacketType type;
        int socket;
        string param0;
        string param1;
        string param2;

        public Packet(string logLine)
        {
            string[] elements = logLine.Split(' ');

            param0 = elements[0];
            Enum.TryParse(elements[0], out type);

            switch (type)
            {
                case PacketType.RECEIVED:
                case PacketType.REQUESTED:
                    param1 = elements[1];
                    break;
                case PacketType.COMPLETED:
                case PacketType.PROCESSED:
                    param1 = elements[1];
                    param2 = elements[3];
                    break;
                case PacketType.TORESOLVE:
                case PacketType.RESOLVED:
                    param1 = elements[1];
                    break;
                default:
                    throw new NonpacketLoglineException("Log line does not match a message exchange");
            }

            socket = Convert.ToInt32(elements[2]);
        }

        public override string ToString()
        {
            return param0 + ' ' + param1 + (type == PacketType.COMPLETED || type == PacketType.PROCESSED ? ' ' + param2 : "");
        }
    }

    [Serializable]
    public class NonpacketLoglineException : Exception
    {
        public NonpacketLoglineException() { }
        public NonpacketLoglineException(string message) : base(message) { }
        public NonpacketLoglineException(string message, Exception inner) : base(message, inner) { }
        protected NonpacketLoglineException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }
}
