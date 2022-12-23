#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint8_t enet_uint8;       /**< unsigned 8-bit type  */
typedef uint16_t enet_uint16;     /**< unsigned 16-bit type */
typedef uint32_t enet_uint32;      /**< unsigned 32-bit type */

#define ENET_API extern

#ifdef _WIN32
    typedef uintptr_t ENetSocket;
    #define ENET_SOCKET_NULL (ENetSocket)(~0)
    #define ENET_CALLBACK __cdecl

   typedef struct
   {
      size_t dataLength;
      void * data;
   } ENetBuffer;

#else
    typedef int ENetSocket;
    #define ENET_SOCKET_NULL -1
    #define ENET_CALLBACK

   typedef struct
   {
      void * data;
      size_t dataLength;
   } ENetBuffer;
#endif

static inline ENetBuffer enet_make_buffer(size_t size, void* data)
{
   ENetBuffer buffer;
   buffer.dataLength = size;
   buffer.data = data;
   return buffer;
}

struct ENetSocketSet;
typedef struct ENetSocketSet ENetSocketSet;

enum
{
   ENET_PROTOCOL_MINIMUM_MTU             = 576,
   ENET_PROTOCOL_MAXIMUM_MTU             = 4096,
   ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS = 32,
   ENET_PROTOCOL_MINIMUM_WINDOW_SIZE     = 4096,
   ENET_PROTOCOL_MAXIMUM_WINDOW_SIZE     = 65536,
   ENET_PROTOCOL_MINIMUM_CHANNEL_COUNT   = 1,
   ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT   = 255,
   ENET_PROTOCOL_MAXIMUM_PEER_ID         = 0xFFF,
   ENET_PROTOCOL_MAXIMUM_FRAGMENT_COUNT  = 1024 * 1024
};

#define ENET_BUFFER_MAXIMUM (1 + 2 * ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS)

typedef enum _ENetProtocolCommand
{
   ENET_PROTOCOL_COMMAND_NONE               = 0,
   ENET_PROTOCOL_COMMAND_ACKNOWLEDGE        = 1,
   ENET_PROTOCOL_COMMAND_CONNECT            = 2,
   ENET_PROTOCOL_COMMAND_VERIFY_CONNECT     = 3,
   ENET_PROTOCOL_COMMAND_DISCONNECT         = 4,
   ENET_PROTOCOL_COMMAND_PING               = 5,
   ENET_PROTOCOL_COMMAND_SEND_RELIABLE      = 6,
   ENET_PROTOCOL_COMMAND_SEND_UNRELIABLE    = 7,
   ENET_PROTOCOL_COMMAND_SEND_FRAGMENT      = 8,
   ENET_PROTOCOL_COMMAND_SEND_UNSEQUENCED   = 9,
   ENET_PROTOCOL_COMMAND_BANDWIDTH_LIMIT    = 10,
   ENET_PROTOCOL_COMMAND_THROTTLE_CONFIGURE = 11,
   ENET_PROTOCOL_COMMAND_SEND_UNRELIABLE_FRAGMENT = 12,
   ENET_PROTOCOL_COMMAND_COUNT              = 13,

   ENET_PROTOCOL_COMMAND_MASK               = 0x0F
} ENetProtocolCommand;

typedef enum _ENetProtocolFlag
{
   ENET_PROTOCOL_COMMAND_FLAG_ACKNOWLEDGE = (1 << 7),
   ENET_PROTOCOL_COMMAND_FLAG_UNSEQUENCED = (1 << 6),

   ENET_PROTOCOL_HEADER_FLAG_COMPRESSED = (1 << 14),
   ENET_PROTOCOL_HEADER_FLAG_SENT_TIME  = (1 << 15),
   ENET_PROTOCOL_HEADER_FLAG_MASK       = ENET_PROTOCOL_HEADER_FLAG_COMPRESSED | ENET_PROTOCOL_HEADER_FLAG_SENT_TIME,

   ENET_PROTOCOL_HEADER_SESSION_MASK    = (3 << 12),
   ENET_PROTOCOL_HEADER_SESSION_SHIFT   = 12
} ENetProtocolFlag;

#ifdef _MSC_VER
#pragma pack(push, 1)
#define ENET_PACKED
#elif defined(__GNUC__) || defined(__clang__)
#define ENET_PACKED __attribute__ ((packed))
#else
#define ENET_PACKED
#endif

typedef struct _ENetProtocolHeader
{
   enet_uint16 peerID;
   enet_uint16 sentTime;
} ENET_PACKED ENetProtocolHeader;

typedef struct _ENetProtocolCommandHeader
{
   enet_uint8 command;
   enet_uint8 channelID;
   enet_uint16 reliableSequenceNumber;
} ENET_PACKED ENetProtocolCommandHeader;

typedef struct _ENetProtocolAcknowledge
{
   ENetProtocolCommandHeader header;
   enet_uint16 receivedReliableSequenceNumber;
   enet_uint16 receivedSentTime;
} ENET_PACKED ENetProtocolAcknowledge;

typedef struct _ENetProtocolConnect
{
   ENetProtocolCommandHeader header;
   enet_uint16 outgoingPeerID;
   enet_uint8  incomingSessionID;
   enet_uint8  outgoingSessionID;
   enet_uint32 mtu;
   enet_uint32 windowSize;
   enet_uint32 channelCount;
   enet_uint32 incomingBandwidth;
   enet_uint32 outgoingBandwidth;
   enet_uint32 packetThrottleInterval;
   enet_uint32 packetThrottleAcceleration;
   enet_uint32 packetThrottleDeceleration;
   enet_uint32 connectID;
   enet_uint32 data;
} ENET_PACKED ENetProtocolConnect;

typedef struct _ENetProtocolVerifyConnect
{
   ENetProtocolCommandHeader header;
   enet_uint16 outgoingPeerID;
   enet_uint8  incomingSessionID;
   enet_uint8  outgoingSessionID;
   enet_uint32 mtu;
   enet_uint32 windowSize;
   enet_uint32 channelCount;
   enet_uint32 incomingBandwidth;
   enet_uint32 outgoingBandwidth;
   enet_uint32 packetThrottleInterval;
   enet_uint32 packetThrottleAcceleration;
   enet_uint32 packetThrottleDeceleration;
   enet_uint32 connectID;
} ENET_PACKED ENetProtocolVerifyConnect;

typedef struct _ENetProtocolBandwidthLimit
{
   ENetProtocolCommandHeader header;
   enet_uint32 incomingBandwidth;
   enet_uint32 outgoingBandwidth;
} ENET_PACKED ENetProtocolBandwidthLimit;

typedef struct _ENetProtocolThrottleConfigure
{
   ENetProtocolCommandHeader header;
   enet_uint32 packetThrottleInterval;
   enet_uint32 packetThrottleAcceleration;
   enet_uint32 packetThrottleDeceleration;
} ENET_PACKED ENetProtocolThrottleConfigure;

typedef struct _ENetProtocolDisconnect
{
   ENetProtocolCommandHeader header;
   enet_uint32 data;
} ENET_PACKED ENetProtocolDisconnect;

typedef struct _ENetProtocolPing
{
   ENetProtocolCommandHeader header;
} ENET_PACKED ENetProtocolPing;

typedef struct _ENetProtocolSendReliable
{
   ENetProtocolCommandHeader header;
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendReliable;

typedef struct _ENetProtocolSendUnreliable
{
   ENetProtocolCommandHeader header;
   enet_uint16 unreliableSequenceNumber;
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendUnreliable;

typedef struct _ENetProtocolSendUnsequenced
{
   ENetProtocolCommandHeader header;
   enet_uint16 unsequencedGroup;
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendUnsequenced;

typedef struct _ENetProtocolSendFragment
{
   ENetProtocolCommandHeader header;
   enet_uint16 startSequenceNumber;
   enet_uint16 dataLength;
   enet_uint32 fragmentCount;
   enet_uint32 fragmentNumber;
   enet_uint32 totalLength;
   enet_uint32 fragmentOffset;
} ENET_PACKED ENetProtocolSendFragment;

typedef union _ENetProtocol
{
   ENetProtocolCommandHeader header;
   ENetProtocolAcknowledge acknowledge;
   ENetProtocolConnect connect;
   ENetProtocolVerifyConnect verifyConnect;
   ENetProtocolDisconnect disconnect;
   ENetProtocolPing ping;
   ENetProtocolSendReliable sendReliable;
   ENetProtocolSendUnreliable sendUnreliable;
   ENetProtocolSendUnsequenced sendUnsequenced;
   ENetProtocolSendFragment sendFragment;
   ENetProtocolBandwidthLimit bandwidthLimit;
   ENetProtocolThrottleConfigure throttleConfigure;
} ENET_PACKED ENetProtocol;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

typedef struct _ENetListNode
{
   struct _ENetListNode * next;
   struct _ENetListNode * previous;
} ENetListNode;

typedef ENetListNode * ENetListIterator;

typedef struct _ENetList
{
   ENetListNode sentinel;
} ENetList;

extern void enet_list_clear (ENetList *);

extern ENetListIterator enet_list_insert (ENetListIterator, void *);
extern void * enet_list_remove (ENetListIterator);
extern ENetListIterator enet_list_move (ENetListIterator, void *, void *);

extern size_t enet_list_size (ENetList *);

#define enet_list_begin(list) ((list) -> sentinel.next)
#define enet_list_end(list) (& (list) -> sentinel)

#define enet_list_empty(list) (enet_list_begin (list) == enet_list_end (list))

#define enet_list_next(iterator) ((iterator) -> next)
#define enet_list_previous(iterator) ((iterator) -> previous)

#define enet_list_front(list) ((void *) (list) -> sentinel.next)
#define enet_list_back(list) ((void *) (list) -> sentinel.previous)

typedef struct _ENetCallbacks
{
    void * (ENET_CALLBACK * malloc) (size_t size);
    void (ENET_CALLBACK * free) (void * memory);
    void (ENET_CALLBACK * no_memory) (void);
} ENetCallbacks;

/** @defgroup callbacks ENet internal callbacks
    @{
    @ingroup private
*/
extern void * enet_malloc (size_t);
extern void   enet_free (void *);

static inline uint16_t ENET_HOST_TO_NET_16(uint16_t value) {
    const uint8_t* p = (uint8_t*)&value;
    return ((uint16_t)p[0] << 8) | (uint16_t)p[1];
}
static inline uint32_t ENET_HOST_TO_NET_32(uint32_t value) {
    const uint8_t* p = (uint8_t*)&value;
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static inline uint16_t ENET_NET_TO_HOST_16(uint16_t value) {
    return ENET_HOST_TO_NET_16(value);
}
static inline uint32_t ENET_NET_TO_HOST_32(uint32_t value) {
    return ENET_HOST_TO_NET_32(value);
}

void ENET_SOCKETSET_EMPTY_PTR(ENetSocketSet* socketSet);
void ENET_SOCKETSET_ADD_PTR(ENetSocketSet* socketSet, ENetSocket socket);
void ENET_SOCKETSET_CLR_PTR(ENetSocketSet* socketSet, ENetSocket socket);
int  ENET_SOCKETSET_CHECK_PTR(ENetSocketSet* socketSet, ENetSocket socket);
#define ENET_SOCKETSET_EMPTY(socketSet) ENET_SOCKETSET_EMPTY_PTR(&socketSet)
#define ENET_SOCKETSET_ADD(socketSet, socket) ENET_SOCKETSET_ADD_PTR(&socketSet, socket)
#define ENET_SOCKETSET_REMOVE(socketSet, socket) ENET_SOCKETSET_REMOVE_PTR(&socketSet, socket)
#define ENET_SOCKETSET_CHECK(socketSet, socket) ENET_SOCKETSET_CHECK_PTR(&socketSet, socket)

#define ENET_MAX(x, y) ((x) > (y) ? (x) : (y))
#define ENET_MIN(x, y) ((x) < (y) ? (x) : (y))
#define ENET_DIFFERENCE(x, y) ((x) < (y) ? (y) - (x) : (x) - (y))


#define ENET_TIME_OVERFLOW 86400000

#define ENET_TIME_LESS(a, b) ((a) - (b) >= ENET_TIME_OVERFLOW)
#define ENET_TIME_GREATER(a, b) ((b) - (a) >= ENET_TIME_OVERFLOW)
#define ENET_TIME_LESS_EQUAL(a, b) (! ENET_TIME_GREATER (a, b))
#define ENET_TIME_GREATER_EQUAL(a, b) (! ENET_TIME_LESS (a, b))

#define ENET_TIME_DIFFERENCE(a, b) ((a) - (b) >= ENET_TIME_OVERFLOW ? (b) - (a) : (a) - (b))


#define ENET_VERSION_MAJOR 1
#define ENET_VERSION_MINOR 3
#define ENET_VERSION_PATCH 17
#define ENET_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define ENET_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define ENET_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define ENET_VERSION_GET_PATCH(version) ((version)&0xFF)
#define ENET_VERSION ENET_VERSION_CREATE(ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH)

typedef enet_uint32 ENetVersion;

struct _ENetHost;
struct _ENetEvent;
struct _ENetPacket;

typedef enum _ENetSocketType
{
   ENET_SOCKET_TYPE_STREAM   = 1,
   ENET_SOCKET_TYPE_DATAGRAM = 2
} ENetSocketType;

typedef enum _ENetSocketWait
{
   ENET_SOCKET_WAIT_NONE      = 0,
   ENET_SOCKET_WAIT_SEND      = (1 << 0),
   ENET_SOCKET_WAIT_RECEIVE   = (1 << 1),
   ENET_SOCKET_WAIT_INTERRUPT = (1 << 2)
} ENetSocketWait;

typedef enum _ENetSocketOption
{
   ENET_SOCKOPT_NONBLOCK  = 1,
   ENET_SOCKOPT_BROADCAST = 2,
   ENET_SOCKOPT_RCVBUF    = 3,
   ENET_SOCKOPT_SNDBUF    = 4,
   ENET_SOCKOPT_REUSEADDR = 5,
   ENET_SOCKOPT_RCVTIMEO  = 6,
   ENET_SOCKOPT_SNDTIMEO  = 7,
   ENET_SOCKOPT_ERROR     = 8,
   ENET_SOCKOPT_NODELAY   = 9
} ENetSocketOption;

typedef enum _ENetSocketShutdown
{
    ENET_SOCKET_SHUTDOWN_READ       = 0,
    ENET_SOCKET_SHUTDOWN_WRITE      = 1,
    ENET_SOCKET_SHUTDOWN_READ_WRITE = 2
} ENetSocketShutdown;

#define ENET_HOST_ANY       0
#define ENET_HOST_BROADCAST 0xFFFFFFFFU
#define ENET_PORT_ANY       0

/**
 * Portable internet address structure. 
 *
 * The host must be specified in network byte-order, and the port must be in host 
 * byte-order. The constant ENET_HOST_ANY may be used to specify the default 
 * server host. The constant ENET_HOST_BROADCAST may be used to specify the
 * broadcast address (255.255.255.255).  This makes sense for enet_host_connect,
 * but not for enet_host_create.  Once a server responds to a broadcast, the
 * address is updated from ENET_HOST_BROADCAST to the server's actual IP address.
 */
typedef struct _ENetAddress
{
   enet_uint32 host;
   enet_uint16 port;
} ENetAddress;

/**
 * Packet flag bit constants.
 *
 * The host must be specified in network byte-order, and the port must be in
 * host byte-order. The constant ENET_HOST_ANY may be used to specify the
 * default server host.
 
   @sa ENetPacket
*/
typedef enum _ENetPacketFlag
{
   /** packet must be received by the target peer and resend attempts should be
     * made until the packet is delivered */
   ENET_PACKET_FLAG_RELIABLE    = (1 << 0),
   /** packet will not be sequenced with other packets
     * not supported for reliable packets
     */
   ENET_PACKET_FLAG_UNSEQUENCED = (1 << 1),
   /** packet will not allocate data, and user must supply it instead */
   ENET_PACKET_FLAG_NO_ALLOCATE = (1 << 2),
   /** packet will be fragmented using unreliable (instead of reliable) sends
     * if it exceeds the MTU */
   ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT = (1 << 3),

   /** whether the packet has been sent from all queues it has been entered into */
   ENET_PACKET_FLAG_SENT = (1<<8)
} ENetPacketFlag;

typedef void (ENET_CALLBACK * ENetPacketFreeCallback) (struct _ENetPacket *);

/**
 * ENet packet structure.
 *
 * An ENet data packet that may be sent to or received from a peer. The shown 
 * fields should only be read and never modified. The data field contains the 
 * allocated data for the packet. The dataLength fields specifies the length 
 * of the allocated data.  The flags field is either 0 (specifying no flags), 
 * or a bitwise-or of any combination of the following flags:
 *
 *    ENET_PACKET_FLAG_RELIABLE - packet must be received by the target peer
 *    and resend attempts should be made until the packet is delivered
 *
 *    ENET_PACKET_FLAG_UNSEQUENCED - packet will not be sequenced with other packets 
 *    (not supported for reliable packets)
 *
 *    ENET_PACKET_FLAG_NO_ALLOCATE - packet will not allocate data, and user must supply it instead
 *
 *    ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT - packet will be fragmented using unreliable
 *    (instead of reliable) sends if it exceeds the MTU
 *
 *    ENET_PACKET_FLAG_SENT - whether the packet has been sent from all queues it has been entered into
   @sa ENetPacketFlag
 */
typedef struct _ENetPacket
{
   size_t                   referenceCount;  /**< internal use only */
   enet_uint32              flags;           /**< bitwise-or of ENetPacketFlag constants */
   enet_uint8 *             data;            /**< allocated data for packet */
   size_t                   dataLength;      /**< length of data */
   ENetPacketFreeCallback   freeCallback;    /**< function to be called when the packet is no longer in use */
   void *                   userData;        /**< application private data, may be freely modified */
} ENetPacket;

typedef struct _ENetAcknowledgement
{
   ENetListNode acknowledgementList;
   enet_uint32  sentTime;
   ENetProtocol command;
} ENetAcknowledgement;

typedef struct _ENetOutgoingCommand
{
   ENetListNode outgoingCommandList;
   enet_uint16  reliableSequenceNumber;
   enet_uint16  unreliableSequenceNumber;
   enet_uint32  sentTime;
   enet_uint32  roundTripTimeout;
   enet_uint32  roundTripTimeoutLimit;
   enet_uint32  fragmentOffset;
   enet_uint16  fragmentLength;
   enet_uint16  sendAttempts;
   ENetProtocol command;
   ENetPacket * packet;
} ENetOutgoingCommand;

typedef struct _ENetIncomingCommand
{  
   ENetListNode     incomingCommandList;
   enet_uint16      reliableSequenceNumber;
   enet_uint16      unreliableSequenceNumber;
   ENetProtocol     command;
   enet_uint32      fragmentCount;
   enet_uint32      fragmentsRemaining;
   enet_uint32 *    fragments;
   ENetPacket *     packet;
} ENetIncomingCommand;

typedef enum _ENetPeerState
{
   ENET_PEER_STATE_DISCONNECTED                = 0,
   ENET_PEER_STATE_CONNECTING                  = 1,
   ENET_PEER_STATE_ACKNOWLEDGING_CONNECT       = 2,
   ENET_PEER_STATE_CONNECTION_PENDING          = 3,
   ENET_PEER_STATE_CONNECTION_SUCCEEDED        = 4,
   ENET_PEER_STATE_CONNECTED                   = 5,
   ENET_PEER_STATE_DISCONNECT_LATER            = 6,
   ENET_PEER_STATE_DISCONNECTING               = 7,
   ENET_PEER_STATE_ACKNOWLEDGING_DISCONNECT    = 8,
   ENET_PEER_STATE_ZOMBIE                      = 9 
} ENetPeerState;

#ifndef ENET_BUFFER_MAXIMUM
#define ENET_BUFFER_MAXIMUM (1 + 2 * ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS)
#endif

enum
{
   ENET_HOST_RECEIVE_BUFFER_SIZE          = 256 * 1024,
   ENET_HOST_SEND_BUFFER_SIZE             = 256 * 1024,
   ENET_HOST_BANDWIDTH_THROTTLE_INTERVAL  = 1000,
   ENET_HOST_DEFAULT_MTU                  = 1400,
   ENET_HOST_DEFAULT_MAXIMUM_PACKET_SIZE  = 32 * 1024 * 1024,
   ENET_HOST_DEFAULT_MAXIMUM_WAITING_DATA = 32 * 1024 * 1024,

   ENET_PEER_DEFAULT_ROUND_TRIP_TIME      = 500,
   ENET_PEER_DEFAULT_PACKET_THROTTLE      = 32,
   ENET_PEER_PACKET_THROTTLE_SCALE        = 32,
   ENET_PEER_PACKET_THROTTLE_COUNTER      = 7, 
   ENET_PEER_PACKET_THROTTLE_ACCELERATION = 2,
   ENET_PEER_PACKET_THROTTLE_DECELERATION = 2,
   ENET_PEER_PACKET_THROTTLE_INTERVAL     = 5000,
   ENET_PEER_PACKET_LOSS_SCALE            = (1 << 16),
   ENET_PEER_PACKET_LOSS_INTERVAL         = 10000,
   ENET_PEER_WINDOW_SIZE_SCALE            = 64 * 1024,
   ENET_PEER_TIMEOUT_LIMIT                = 32,
   ENET_PEER_TIMEOUT_MINIMUM              = 5000,
   ENET_PEER_TIMEOUT_MAXIMUM              = 30000,
   ENET_PEER_PING_INTERVAL                = 500,
   ENET_PEER_UNSEQUENCED_WINDOWS          = 64,
   ENET_PEER_UNSEQUENCED_WINDOW_SIZE      = 1024,
   ENET_PEER_FREE_UNSEQUENCED_WINDOWS     = 32,
   ENET_PEER_RELIABLE_WINDOWS             = 16,
   ENET_PEER_RELIABLE_WINDOW_SIZE         = 0x1000,
   ENET_PEER_FREE_RELIABLE_WINDOWS        = 8
};

typedef struct _ENetChannel
{
   enet_uint16  outgoingReliableSequenceNumber;
   enet_uint16  outgoingUnreliableSequenceNumber;
   enet_uint16  usedReliableWindows;
   enet_uint16  reliableWindows [ENET_PEER_RELIABLE_WINDOWS];
   enet_uint16  incomingReliableSequenceNumber;
   enet_uint16  incomingUnreliableSequenceNumber;
   ENetList     incomingReliableCommands;
   ENetList     incomingUnreliableCommands;
} ENetChannel;

typedef enum _ENetPeerFlag
{
   ENET_PEER_FLAG_NEEDS_DISPATCH = (1 << 0)
} ENetPeerFlag;

/**
 * An ENet peer which data packets may be sent or received from. 
 *
 * No fields should be modified unless otherwise specified. 
 */
typedef struct _ENetPeer
{ 
   ENetListNode  dispatchList;
   struct _ENetHost * host;
   enet_uint16   outgoingPeerID;
   enet_uint16   incomingPeerID;
   enet_uint32   connectID;
   enet_uint8    outgoingSessionID;
   enet_uint8    incomingSessionID;
   ENetAddress   address;            /**< Internet address of the peer */
   void *        data;               /**< Application private data, may be freely modified */
   ENetPeerState state;
   ENetChannel * channels;
   size_t        channelCount;       /**< Number of channels allocated for communication with peer */
   enet_uint32   incomingBandwidth;  /**< Downstream bandwidth of the client in bytes/second */
   enet_uint32   outgoingBandwidth;  /**< Upstream bandwidth of the client in bytes/second */
   enet_uint32   incomingBandwidthThrottleEpoch;
   enet_uint32   outgoingBandwidthThrottleEpoch;
   enet_uint32   incomingDataTotal;
   enet_uint32   outgoingDataTotal;
   enet_uint32   lastSendTime;
   enet_uint32   lastReceiveTime;
   enet_uint32   nextTimeout;
   enet_uint32   earliestTimeout;
   enet_uint32   packetLossEpoch;
   enet_uint32   packetsSent;
   enet_uint32   packetsLost;
   enet_uint32   packetLoss;          /**< mean packet loss of reliable packets as a ratio with respect to the constant ENET_PEER_PACKET_LOSS_SCALE */
   enet_uint32   packetLossVariance;
   enet_uint32   packetThrottle;
   enet_uint32   packetThrottleLimit;
   enet_uint32   packetThrottleCounter;
   enet_uint32   packetThrottleEpoch;
   enet_uint32   packetThrottleAcceleration;
   enet_uint32   packetThrottleDeceleration;
   enet_uint32   packetThrottleInterval;
   enet_uint32   pingInterval;
   enet_uint32   timeoutLimit;
   enet_uint32   timeoutMinimum;
   enet_uint32   timeoutMaximum;
   enet_uint32   lastRoundTripTime;
   enet_uint32   lowestRoundTripTime;
   enet_uint32   lastRoundTripTimeVariance;
   enet_uint32   highestRoundTripTimeVariance;
   enet_uint32   roundTripTime;            /**< mean round trip time (RTT), in milliseconds, between sending a reliable packet and receiving its acknowledgement */
   enet_uint32   roundTripTimeVariance;
   enet_uint32   mtu;
   enet_uint32   windowSize;
   enet_uint32   reliableDataInTransit;
   enet_uint16   outgoingReliableSequenceNumber;
   ENetList      acknowledgements;
   ENetList      sentReliableCommands;
   ENetList      sentUnreliableCommands;
   ENetList      outgoingCommands;
   ENetList      dispatchedCommands;
   enet_uint16   flags;
   enet_uint16   reserved;
   enet_uint16   incomingUnsequencedGroup;
   enet_uint16   outgoingUnsequencedGroup;
   enet_uint32   unsequencedWindow [ENET_PEER_UNSEQUENCED_WINDOW_SIZE / 32]; 
   enet_uint32   eventData;
   size_t        totalWaitingData;
} ENetPeer;

/** An ENet packet compressor for compressing UDP packets before socket sends or receives.
 */
typedef struct _ENetCompressor
{
   /** Context data for the compressor. Must be non-NULL. */
   void * context;
   /** Compresses from inBuffers[0:inBufferCount-1], containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure. */
   size_t (ENET_CALLBACK * compress) (void * context, const ENetBuffer * inBuffers, size_t inBufferCount, size_t inLimit, enet_uint8 * outData, size_t outLimit);
   /** Decompresses from inData, containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure. */
   size_t (ENET_CALLBACK * decompress) (void * context, const enet_uint8 * inData, size_t inLimit, enet_uint8 * outData, size_t outLimit);
   /** Destroys the context when compression is disabled or the host is destroyed. May be NULL. */
   void (ENET_CALLBACK * destroy) (void * context);
} ENetCompressor;

/** Callback that computes the checksum of the data held in buffers[0:bufferCount-1] */
typedef enet_uint32 (ENET_CALLBACK * ENetChecksumCallback) (const ENetBuffer * buffers, size_t bufferCount);

/** Callback for intercepting received raw UDP packets. Should return 1 to intercept, 0 to ignore, or -1 to propagate an error. */
typedef int (ENET_CALLBACK * ENetInterceptCallback) (struct _ENetHost * host, struct _ENetEvent * event);
 
/** An ENet host for communicating with peers.
  *
  * No fields should be modified unless otherwise stated.

    @sa enet_host_create()
    @sa enet_host_destroy()
    @sa enet_host_connect()
    @sa enet_host_service()
    @sa enet_host_flush()
    @sa enet_host_broadcast()
    @sa enet_host_compress()
    @sa enet_host_compress_with_range_coder()
    @sa enet_host_channel_limit()
    @sa enet_host_bandwidth_limit()
    @sa enet_host_bandwidth_throttle()
  */
typedef struct _ENetHost
{
   ENetSocket           socket;
   ENetAddress          address;                     /**< Internet address of the host */
   enet_uint32          incomingBandwidth;           /**< downstream bandwidth of the host */
   enet_uint32          outgoingBandwidth;           /**< upstream bandwidth of the host */
   enet_uint32          bandwidthThrottleEpoch;
   enet_uint32          mtu;
   enet_uint32          randomSeed;
   int                  recalculateBandwidthLimits;
   ENetPeer *           peers;                       /**< array of peers allocated for this host */
   size_t               peerCount;                   /**< number of peers allocated for this host */
   size_t               channelLimit;                /**< maximum number of channels allowed for connected peers */
   enet_uint32          serviceTime;
   ENetList             dispatchQueue;
   int                  continueSending;
   size_t               packetSize;
   enet_uint16          headerFlags;
   ENetProtocol         commands [ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS];
   size_t               commandCount;
   ENetBuffer           buffers [ENET_BUFFER_MAXIMUM];
   size_t               bufferCount;
   ENetChecksumCallback checksum;                    /**< callback the user can set to enable packet checksums for this host */
   ENetCompressor       compressor;
   enet_uint8           packetData [2][ENET_PROTOCOL_MAXIMUM_MTU];
   ENetAddress          receivedAddress;
   enet_uint8 *         receivedData;
   size_t               receivedDataLength;
   enet_uint32          totalSentData;               /**< total data sent, user should reset to 0 as needed to prevent overflow */
   enet_uint32          totalSentPackets;            /**< total UDP packets sent, user should reset to 0 as needed to prevent overflow */
   enet_uint32          totalReceivedData;           /**< total data received, user should reset to 0 as needed to prevent overflow */
   enet_uint32          totalReceivedPackets;        /**< total UDP packets received, user should reset to 0 as needed to prevent overflow */
   ENetInterceptCallback intercept;                  /**< callback the user can set to intercept received raw UDP packets */
   size_t               connectedPeers;
   size_t               bandwidthLimitedPeers;
   size_t               duplicatePeers;              /**< optional number of allowed peers from duplicate IPs, defaults to ENET_PROTOCOL_MAXIMUM_PEER_ID */
   size_t               maximumPacketSize;           /**< the maximum allowable packet size that may be sent or received on a peer */
   size_t               maximumWaitingData;          /**< the maximum aggregate amount of buffer space a peer may use waiting for packets to be delivered */
} ENetHost;

/**
 * An ENet event type, as specified in @ref ENetEvent.
 */
typedef enum _ENetEventType
{
   /** no event occurred within the specified time limit */
   ENET_EVENT_TYPE_NONE       = 0,  

   /** a connection request initiated by enet_host_connect has completed.  
     * The peer field contains the peer which successfully connected. 
     */
   ENET_EVENT_TYPE_CONNECT    = 1,  

   /** a peer has disconnected.  This event is generated on a successful 
     * completion of a disconnect initiated by enet_peer_disconnect, if 
     * a peer has timed out, or if a connection request intialized by 
     * enet_host_connect has timed out.  The peer field contains the peer 
     * which disconnected. The data field contains user supplied data 
     * describing the disconnection, or 0, if none is available.
     */
   ENET_EVENT_TYPE_DISCONNECT = 2,  

   /** a packet has been received from a peer.  The peer field specifies the
     * peer which sent the packet.  The channelID field specifies the channel
     * number upon which the packet was received.  The packet field contains
     * the packet that was received; this packet must be destroyed with
     * enet_packet_destroy after use.
     */
   ENET_EVENT_TYPE_RECEIVE    = 3
} ENetEventType;

/**
 * An ENet event as returned by enet_host_service().
   
   @sa enet_host_service
 */
typedef struct _ENetEvent 
{
   ENetEventType        type;      /**< type of the event */
   ENetPeer *           peer;      /**< peer that generated a connect, disconnect or receive event */
   enet_uint8           channelID; /**< channel on the peer that generated the event, if appropriate */
   enet_uint32          data;      /**< data associated with the event, if appropriate */
   ENetPacket *         packet;    /**< packet associated with the event, if appropriate */
} ENetEvent;

/** @defgroup global ENet global functions
    @{ 
*/

/** 
  Initializes ENet globally.  Must be called prior to using any functions in
  ENet.
  @returns 0 on success, < 0 on failure
*/
ENET_API int enet_initialize (void);

/** 
  Initializes ENet globally and supplies user-overridden callbacks. Must be called prior to using any functions in ENet. Do not use enet_initialize() if you use this variant. Make sure the ENetCallbacks structure is zeroed out so that any additional callbacks added in future versions will be properly ignored.

  @param version the constant ENET_VERSION should be supplied so ENet knows which version of ENetCallbacks struct to use
  @param inits user-overridden callbacks where any NULL callbacks will use ENet's defaults
  @returns 0 on success, < 0 on failure
*/
ENET_API int enet_initialize_with_callbacks (ENetVersion version, const ENetCallbacks * inits);

/** 
  Shuts down ENet globally.  Should be called when a program that has
  initialized ENet exits.
*/
ENET_API void enet_deinitialize (void);

/**
  Gives the linked version of the ENet library.
  @returns the version number 
*/
ENET_API ENetVersion enet_linked_version (void);

/** @} */

/** @defgroup private ENet private implementation functions */

/**
  Returns the wall-time in milliseconds.  Its initial value is unspecified
  unless otherwise set.
  */
ENET_API enet_uint32 enet_time_get (void);
/**
  Sets the current wall-time in milliseconds.
  */
ENET_API void enet_time_set (enet_uint32);

/** @defgroup socket ENet socket functions
    @{
*/
ENET_API ENetSocket enet_socket_create (ENetSocketType);
ENET_API int        enet_socket_bind (ENetSocket, const ENetAddress *);
ENET_API int        enet_socket_get_address (ENetSocket, ENetAddress *);
ENET_API int        enet_socket_listen (ENetSocket, int);
ENET_API ENetSocket enet_socket_accept (ENetSocket, ENetAddress *);
ENET_API int        enet_socket_connect (ENetSocket, const ENetAddress *);
ENET_API int        enet_socket_send (ENetSocket, const ENetAddress *, const ENetBuffer *, size_t);
ENET_API int        enet_socket_receive (ENetSocket, ENetAddress *, ENetBuffer *, size_t);
ENET_API int        enet_socket_wait (ENetSocket, enet_uint32 *, enet_uint32);
ENET_API int        enet_socket_set_option (ENetSocket, ENetSocketOption, int);
ENET_API int        enet_socket_get_option (ENetSocket, ENetSocketOption, int *);
ENET_API int        enet_socket_shutdown (ENetSocket, ENetSocketShutdown);
ENET_API void       enet_socket_destroy (ENetSocket);
ENET_API int        enet_socketset_select (ENetSocket, ENetSocketSet *, ENetSocketSet *, enet_uint32);

/** @} */

/** @defgroup Address ENet address functions
    @{
*/

/** Attempts to parse the printable form of the IP address in the parameter hostName
    and sets the host field in the address parameter if successful.
    @param address destination to store the parsed IP address
    @param hostName IP address to parse
    @retval 0 on success
    @retval < 0 on failure
    @returns the address of the given hostName in address on success
*/
ENET_API int enet_address_set_host_ip (ENetAddress * address, const char * hostName);

/** Attempts to resolve the host named by the parameter hostName and sets
    the host field in the address parameter if successful.
    @param address destination to store resolved address
    @param hostName host name to lookup
    @retval 0 on success
    @retval < 0 on failure
    @returns the address of the given hostName in address on success
*/
ENET_API int enet_address_set_host (ENetAddress * address, const char * hostName);

/** Gives the printable form of the IP address specified in the address parameter.
    @param address    address printed
    @param hostName   destination for name, must not be NULL
    @param nameLength maximum length of hostName.
    @returns the null-terminated name of the host in hostName on success
    @retval 0 on success
    @retval < 0 on failure
*/
ENET_API int enet_address_get_host_ip (const ENetAddress * address, char * hostName, size_t nameLength);

/** Attempts to do a reverse lookup of the host field in the address parameter.
    @param address    address used for reverse lookup
    @param hostName   destination for name, must not be NULL
    @param nameLength maximum length of hostName.
    @returns the null-terminated name of the host in hostName on success
    @retval 0 on success
    @retval < 0 on failure
*/
ENET_API int enet_address_get_host (const ENetAddress * address, char * hostName, size_t nameLength);

/** @} */

ENET_API ENetPacket * enet_packet_create (const void *, size_t, enet_uint32);
ENET_API void         enet_packet_destroy (ENetPacket *);
ENET_API int          enet_packet_resize  (ENetPacket *, size_t);
ENET_API enet_uint32  enet_crc32 (const ENetBuffer *, size_t);
                
ENET_API ENetHost * enet_host_create (const ENetAddress *, size_t, size_t, enet_uint32, enet_uint32);
ENET_API void       enet_host_destroy (ENetHost *);
ENET_API ENetPeer * enet_host_connect (ENetHost *, const ENetAddress *, size_t, enet_uint32);
ENET_API int        enet_host_check_events (ENetHost *, ENetEvent *);
ENET_API int        enet_host_service (ENetHost *, ENetEvent *, enet_uint32);
ENET_API void       enet_host_flush (ENetHost *);
ENET_API void       enet_host_broadcast (ENetHost *, enet_uint8, ENetPacket *);
ENET_API void       enet_host_compress (ENetHost *, const ENetCompressor *);
ENET_API int        enet_host_compress_with_range_coder (ENetHost * host);
ENET_API void       enet_host_channel_limit (ENetHost *, size_t);
ENET_API void       enet_host_bandwidth_limit (ENetHost *, enet_uint32, enet_uint32);
extern   void       enet_host_bandwidth_throttle (ENetHost *);
extern  enet_uint32 enet_host_random_seed (void);
extern  enet_uint32 enet_host_random (ENetHost *);

ENET_API int                 enet_peer_send (ENetPeer *, enet_uint8, ENetPacket *);
ENET_API ENetPacket *        enet_peer_receive (ENetPeer *, enet_uint8 * channelID);
ENET_API void                enet_peer_ping (ENetPeer *);
ENET_API void                enet_peer_ping_interval (ENetPeer *, enet_uint32);
ENET_API void                enet_peer_timeout (ENetPeer *, enet_uint32, enet_uint32, enet_uint32);
ENET_API void                enet_peer_reset (ENetPeer *);
ENET_API void                enet_peer_disconnect (ENetPeer *, enet_uint32);
ENET_API void                enet_peer_disconnect_now (ENetPeer *, enet_uint32);
ENET_API void                enet_peer_disconnect_later (ENetPeer *, enet_uint32);
ENET_API void                enet_peer_throttle_configure (ENetPeer *, enet_uint32, enet_uint32, enet_uint32);
extern int                   enet_peer_throttle (ENetPeer *, enet_uint32);
extern void                  enet_peer_reset_queues (ENetPeer *);
extern void                  enet_peer_setup_outgoing_command (ENetPeer *, ENetOutgoingCommand *);
extern ENetOutgoingCommand * enet_peer_queue_outgoing_command (ENetPeer *, const ENetProtocol *, ENetPacket *, enet_uint32, enet_uint16);
extern ENetIncomingCommand * enet_peer_queue_incoming_command (ENetPeer *, const ENetProtocol *, const void *, size_t, enet_uint32, enet_uint32);
extern ENetAcknowledgement * enet_peer_queue_acknowledgement (ENetPeer *, const ENetProtocol *, enet_uint16);
extern void                  enet_peer_dispatch_incoming_unreliable_commands (ENetPeer *, ENetChannel *, ENetIncomingCommand *);
extern void                  enet_peer_dispatch_incoming_reliable_commands (ENetPeer *, ENetChannel *, ENetIncomingCommand *);
extern void                  enet_peer_on_connect (ENetPeer *);
extern void                  enet_peer_on_disconnect (ENetPeer *);

ENET_API void * enet_range_coder_create (void);
ENET_API void   enet_range_coder_destroy (void *);
ENET_API size_t enet_range_coder_compress (void *, const ENetBuffer *, size_t, size_t, enet_uint8 *, size_t);
ENET_API size_t enet_range_coder_decompress (void *, const enet_uint8 *, size_t, enet_uint8 *, size_t);
   
extern size_t enet_protocol_command_size (enet_uint8);

ENET_API enet_uint32 enet_get_lan_broadcast_ip_addresses(enet_uint32 maxAddresses, enet_uint32* addresses);

#ifdef __cplusplus
}
#endif