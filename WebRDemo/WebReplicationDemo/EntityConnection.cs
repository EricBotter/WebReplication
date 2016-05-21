using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebReplicationDemo
{
    class EntityConnection
    {
        NetworkEntity client;
        NetworkEntity server;

        List<Packet> packets = new List<Packet>();
        bool active = false;

        EntityConnection(NetworkEntity client, int port)
        {
            this.client = client;
            this.server = NetworkEntity.allEntities.AsEnumerable().FirstOrDefault(x => x.port == port);
        }

        void addPacket(Packet p)
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
            EVADED,
            TORESOLVE,
            RESOLVED
        }

        PacketType type;
        int socket;
        string param1;
        string param2;

        Packet(string logLine)
        {
            string[] elements = logLine.Split(' ');

            Enum.TryParse(elements[0], out type);

            switch (type)
            {
                case PacketType.RECEIVED:
                case PacketType.REQUESTED:
                    param1 = elements[1];
                    break;
                case PacketType.COMPLETED:
                case PacketType.EVADED:
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
