from ctypes import *

STRING = c_char_p
_libraries = {}
_libraries['libsoem.so'] = CDLL('libsoem.so')


ECT_COES_RXPDO = 5
ECT_COES_TXPDO = 4
ECT_COES_SDORES = 3
ECT_COES_SDOREQ = 2
ECT_COES_EMERGENCY = 1
ECT_MBXT_VOE = 15
ECT_MBXT_SOE = 5
ECT_MBXT_FOE = 4
ECT_UNSIGNED24 = 22
ECT_INTEGER64 = 21
ECT_BIT6 = 53
ECT_MBXT_AOE = 1
ECT_MBXT_ERR = 0
ECT_DOMAIN = 15
ECT_TIME_DIFFERENCE = 13
ECT_SII_RXMBXADR = 24
ECT_SII_TXMBXADR = 26
ECT_BIT5 = 52
ECT_SII_MBXSIZE = 25
ECT_SII_BOOTTXMBX = 22
ECT_SII_BOOTRXMBX = 20
ECT_SII_REV = 12
ECT_SII_ID = 10
ECT_UNSIGNED8 = 5
ECT_BIT4 = 51
ECT_INTEGER32 = 4
ECT_INTEGER16 = 3
ECT_BIT3 = 50
EC_ECMD_RELOAD = 768
ECT_BIT2 = 49
ECT_BIT1 = 48
EC_ECMD_READ = 256
ECT_SOE_NOTIFICATION = 5
ECT_UNSIGNED64 = 27
ECT_MBXT_COE = 3
ECT_MBXT_EOE = 2
EC_STATE_ERROR = 16
EC_STATE_ACK = 16
EC_STATE_OPERATIONAL = 8
ECT_SII_MANUF = 8
ECT_REAL64 = 17
EC_STATE_BOOT = 3
EC_STATE_PRE_OP = 2
EC_STATE_INIT = 1
ECT_SOE_READREQ = 1
ECT_INTEGER24 = 16
ECT_SII_MBXPROTO = 28
ECT_TIME_OF_DAY = 12
ECT_SOE_EMERGENCY = 6
ECT_SOE_WRITERES = 4
ECT_UNICODE_STRING = 11
ECT_SOE_WRITEREQ = 3
ECT_SOE_READRES = 2
ECT_SDOINFO_ERROR = 7
ECT_OCTET_STRING = 10
ECT_GET_OE_RES = 6
ECT_BIT7 = 54
ECT_GET_OE_REQ = 5
ECT_GET_OD_REQ = 3
ECT_GET_OD_RES = 4
ECT_GET_ODLIST_RES = 2
ECT_GET_ODLIST_REQ = 1
ECT_VISIBLE_STRING = 9
ECT_SDO_SEG_UP_REQ = 96
ECT_SDO_UP_REQ_CA = 80
ECT_SDO_UP_REQ = 64
ECT_SDO_DOWN_INIT_CA = 49
ECT_SDO_DOWN_INIT = 33
ECT_REAL32 = 8
ECT_SII_PDO = 50
ECT_SII_SM = 41
ECT_SII_FMMU = 40
ECT_SII_GENERAL = 30
ECT_SII_STRING = 10
ECT_UNSIGNED32 = 7
ECT_UNSIGNED16 = 6
ECT_SDO_ABORT = 128
EC_CMD_FRMW = 14
EC_BUF_COMPLETE = 4
EC_CMD_LWR = 11
EC_CMD_LRD = 10
EC_CMD_BRD = 7
EC_BUF_RCVD = 3
EC_CMD_FPWR = 5
EC_CMD_FPRD = 4
EC_CMD_APRW = 3
EC_CMD_APWR = 2
EC_CMD_NOP = 0
EC_BUF_TX = 2
EC_BUF_ALLOC = 1
ECT_REG_DCCYCLE0 = 2464
EC_BUF_EMPTY = 0
EC_ECMD_WRITE = 513
EC_ECMD_NOP = 0
ECT_REG_DCTIMEFILT = 2356
ECT_REG_DCSPEEDCNT = 2352
ECT_REG_DCSYSOFFSET = 2336
EC_ERR_TYPE_SOE_ERROR = 8
EC_ERR_TYPE_FOE_PACKETNUMBER = 7
EC_ERR_TYPE_FOE_BUF2SMALL = 6
EC_ERR_TYPE_FOE_ERROR = 5
EC_ERR_TYPE_SDOINFO_ERROR = 4
EC_ERR_TYPE_PACKET_ERROR = 3
EC_ERR_TYPE_EMERGENCY = 1
EC_ERR_TYPE_SDO_ERROR = 0
ITIMER_PROF = 2
ITIMER_VIRTUAL = 1
ITIMER_REAL = 0
ECT_INTEGER8 = 2
ECT_BOOLEAN = 1
EC_STATE_SAFE_OP = 4
EC_CMD_ARMW = 13
EC_CMD_LRW = 12
EC_CMD_BRW = 9
EC_CMD_BWR = 8
EC_CMD_FPRW = 6
EC_ERR_NOK = 5
EC_ERR_NO_SLAVES = 4
EC_ERR_TIMEOUT = 3
EC_ERR_NOT_INITIALIZED = 2
EC_ERR_ALREADY_INITIALIZED = 1
EC_ERR_OK = 0
ECT_BIT8 = 55
ECT_REG_DCCYCLE1 = 2468
ECT_REG_DCSTART0 = 2448
ECT_REG_DCSYNCACT = 2433
ECT_REG_DCCUC = 2432
ECT_REG_DCSYSDIFF = 2348
ECT_REG_DCSYSDELAY = 2344
EC_CMD_APRD = 1
ECT_REG_DCSOF = 2328
ECT_REG_DCSYSTIME = 2320
ECT_REG_DCTIME3 = 2316
ECT_REG_DCTIME2 = 2312
ECT_REG_DCTIME1 = 2308
ECT_REG_DCTIME0 = 2304
ECT_REG_SM1CONTR = 2063
ECT_REG_SM1ACT = 2062
ECT_REG_SM1STAT = 2061
ECT_REG_SM0STAT = 2053
ECT_REG_SM3 = 2072
ECT_REG_SM2 = 2064
ECT_REG_SM1 = 2056
ECT_REG_SM0 = 2048
ECT_REG_FMMU3 = 1584
ECT_REG_FMMU2 = 1568
ECT_REG_FMMU1 = 1552
ECT_REG_FMMU0 = 1536
ECT_REG_EEPDAT = 1288
ECT_REG_EEPADR = 1284
ECT_REG_EEPSTAT = 1282
ECT_REG_EEPCTL = 1282
ECT_REG_EEPCFG = 1280
ECT_REG_RXERR = 768
ECT_REG_IRQMASK = 512
ECT_REG_PDICTL = 320
ECT_REG_ALSTATCODE = 308
ECT_REG_ALSTAT = 304
ECT_REG_ALCTL = 288
ECT_REG_DLSTAT = 272
ECT_REG_DLALIAS = 259
ECT_REG_DLPORT = 257
ECT_REG_DLCTL = 256
ECT_REG_ALIAS = 18
ECT_REG_STADR = 16
ECT_REG_ESCSUP = 8
ECT_REG_PORTDES = 7
ECT_REG_TYPE = 0
ECT_FOE_BUSY = 6
ECT_FOE_ERROR = 5
ECT_FOE_ACK = 4
ECT_FOE_DATA = 3
ECT_FOE_WRITE = 2
ECT_FOE_READ = 1
ECT_COES_SDOINFO = 8
ECT_COES_RXPDO_RR = 7
ECT_COES_TXPDO_RR = 6
__time_t = c_long
time_t = __time_t
class timespec(Structure):
    pass
timespec._fields_ = [
    ('tv_sec', __time_t),
    ('tv_nsec', c_long),
]
__sig_atomic_t = c_int
class __sigset_t(Structure):
    pass
__sigset_t._fields_ = [
    ('__val', c_ulong * 16),
]
class timeval(Structure):
    pass
__suseconds_t = c_long
timeval._fields_ = [
    ('tv_sec', __time_t),
    ('tv_usec', __suseconds_t),
]
__u_char = c_ubyte
__u_short = c_ushort
__u_int = c_uint
__u_long = c_ulong
__int8_t = c_byte
__uint8_t = c_ubyte
__int16_t = c_short
__uint16_t = c_ushort
__int32_t = c_int
__uint32_t = c_uint
__int64_t = c_long
__uint64_t = c_ulong
__quad_t = c_long
__u_quad_t = c_ulong
__dev_t = c_ulong
__uid_t = c_uint
__gid_t = c_uint
__ino_t = c_ulong
__ino64_t = c_ulong
__mode_t = c_uint
__nlink_t = c_ulong
__off_t = c_long
__off64_t = c_long
__pid_t = c_int
class __fsid_t(Structure):
    pass
__fsid_t._fields_ = [
    ('__val', c_int * 2),
]
__clock_t = c_long
__rlim_t = c_ulong
__rlim64_t = c_ulong
__id_t = c_uint
__useconds_t = c_uint
__daddr_t = c_int
__swblk_t = c_long
__key_t = c_int
__clockid_t = c_int
__timer_t = c_void_p
__blksize_t = c_long
__blkcnt_t = c_long
__blkcnt64_t = c_long
__fsblkcnt_t = c_ulong
__fsblkcnt64_t = c_ulong
__fsfilcnt_t = c_ulong
__fsfilcnt64_t = c_ulong
__ssize_t = c_long
__loff_t = __off64_t
__qaddr_t = POINTER(__quad_t)
__caddr_t = STRING
__intptr_t = c_long
__socklen_t = c_uint
sigset_t = __sigset_t
suseconds_t = __suseconds_t
__fd_mask = c_long
class fd_set(Structure):
    pass
fd_set._fields_ = [
    ('fds_bits', __fd_mask * 16),
]
fd_mask = __fd_mask
select = _libraries['libsoem.so'].select
select.restype = c_int
select.argtypes = [c_int, POINTER(fd_set), POINTER(fd_set), POINTER(fd_set), POINTER(timeval)]
pselect = _libraries['libsoem.so'].pselect
pselect.restype = c_int
pselect.argtypes = [c_int, POINTER(fd_set), POINTER(fd_set), POINTER(fd_set), POINTER(timespec), POINTER(__sigset_t)]
class timezone(Structure):
    pass
timezone._fields_ = [
    ('tz_minuteswest', c_int),
    ('tz_dsttime', c_int),
]
__timezone_ptr_t = POINTER(timezone)
gettimeofday = _libraries['libsoem.so'].gettimeofday
gettimeofday.restype = c_int
gettimeofday.argtypes = [POINTER(timeval), __timezone_ptr_t]
settimeofday = _libraries['libsoem.so'].settimeofday
settimeofday.restype = c_int
settimeofday.argtypes = [POINTER(timeval), POINTER(timezone)]
adjtime = _libraries['libsoem.so'].adjtime
adjtime.restype = c_int
adjtime.argtypes = [POINTER(timeval), POINTER(timeval)]

# values for enumeration '__itimer_which'
__itimer_which = c_int # enum
class itimerval(Structure):
    pass
itimerval._fields_ = [
    ('it_interval', timeval),
    ('it_value', timeval),
]
__itimer_which_t = c_int
getitimer = _libraries['libsoem.so'].getitimer
getitimer.restype = c_int
getitimer.argtypes = [__itimer_which_t, POINTER(itimerval)]
setitimer = _libraries['libsoem.so'].setitimer
setitimer.restype = c_int
setitimer.argtypes = [__itimer_which_t, POINTER(itimerval), POINTER(itimerval)]
utimes = _libraries['libsoem.so'].utimes
utimes.restype = c_int
utimes.argtypes = [STRING, POINTER(timeval)]
lutimes = _libraries['libsoem.so'].lutimes
lutimes.restype = c_int
lutimes.argtypes = [STRING, POINTER(timeval)]
futimes = _libraries['libsoem.so'].futimes
futimes.restype = c_int
futimes.argtypes = [c_int, POINTER(timeval)]
futimesat = _libraries['libsoem.so'].futimesat
futimesat.restype = c_int
futimesat.argtypes = [c_int, STRING, POINTER(timeval)]
class ec_SoEnamet(Structure):
    pass
uint16 = c_ushort
ec_SoEnamet._pack_ = 1
ec_SoEnamet._fields_ = [
    ('currentlength', uint16),
    ('maxlength', uint16),
    ('name', c_char * 60),
]
class N11ec_SoElistt4DOT_26E(Union):
    pass
uint8 = c_ubyte
uint32 = c_uint
uint64 = c_ulonglong
N11ec_SoElistt4DOT_26E._fields_ = [
    ('byte', uint8 * 8),
    ('word', uint16 * 4),
    ('dword', uint32 * 2),
    ('lword', uint64 * 1),
]
class ec_SoElistt(Structure):
    pass
ec_SoElistt._pack_ = 1
ec_SoElistt._anonymous_ = ['_0']
ec_SoElistt._fields_ = [
    ('currentlength', uint16),
    ('maxlength', uint16),
    ('_0', N11ec_SoElistt4DOT_26E),
]
class ec_SoEmappingt(Structure):
    pass
ec_SoEmappingt._pack_ = 1
ec_SoEmappingt._fields_ = [
    ('currentlength', uint16),
    ('maxlength', uint16),
    ('idn', uint16 * 64),
]
class ec_SoEattributet(Structure):
    pass
ec_SoEattributet._fields_ = [
    ('evafactor', uint32, 16),
    ('length', uint32, 2),
    ('list', uint32, 1),
    ('command', uint32, 1),
    ('datatype', uint32, 3),
    ('reserved1', uint32, 1),
    ('decimals', uint32, 4),
    ('wppreop', uint32, 1),
    ('wpsafeop', uint32, 1),
    ('wpop', uint32, 1),
    ('reserved2', uint32, 1),
]
ec_SoEread = _libraries['libsoem.so'].ec_SoEread
ec_SoEread.restype = c_int
ec_SoEread.argtypes = [uint16, uint8, uint8, uint16, POINTER(c_int), c_void_p, c_int]
ec_SoEwrite = _libraries['libsoem.so'].ec_SoEwrite
ec_SoEwrite.restype = c_int
ec_SoEwrite.argtypes = [uint16, uint8, uint8, uint16, c_int, c_void_p, c_int]
ec_readIDNmap = _libraries['libsoem.so'].ec_readIDNmap
ec_readIDNmap.restype = c_int
ec_readIDNmap.argtypes = [uint16, POINTER(c_int), POINTER(c_int)]
boolean = c_ubyte
int8 = c_byte
int16 = c_short
int32 = c_int
int64 = c_longlong
ec_bufT = uint8 * 1518
class ec_etherheadert(Structure):
    pass
ec_etherheadert._pack_ = 1
ec_etherheadert._fields_ = [
    ('da0', uint16),
    ('da1', uint16),
    ('da2', uint16),
    ('sa0', uint16),
    ('sa1', uint16),
    ('sa2', uint16),
    ('etype', uint16),
]
class ec_comt(Structure):
    pass
ec_comt._pack_ = 1
ec_comt._fields_ = [
    ('elength', uint16),
    ('command', uint8),
    ('index', uint8),
    ('ADP', uint16),
    ('ADO', uint16),
    ('dlength', uint16),
    ('irpt', uint16),
]

# values for enumeration 'ec_err'
ec_err = c_int # enum

# values for enumeration 'ec_state'
ec_state = c_int # enum

# values for enumeration 'ec_bufstate'
ec_bufstate = c_int # enum

# values for enumeration 'ec_datatype'
ec_datatype = c_int # enum

# values for enumeration 'ec_cmdtype'
ec_cmdtype = c_int # enum

# values for enumeration 'ec_ecmdtype'
ec_ecmdtype = c_int # enum

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for unnamed enumeration

# values for enumeration 'ec_err_type'
ec_err_type = c_int # enum
class N9ec_errort4DOT_22E(Union):
    pass
class N9ec_errort4DOT_224DOT_23E(Structure):
    pass
N9ec_errort4DOT_224DOT_23E._fields_ = [
    ('ErrorCode', uint16),
    ('ErrorReg', uint8),
    ('b1', uint8),
    ('w1', uint16),
    ('w2', uint16),
]
N9ec_errort4DOT_22E._anonymous_ = ['_0']
N9ec_errort4DOT_22E._fields_ = [
    ('AbortCode', int32),
    ('_0', N9ec_errort4DOT_224DOT_23E),
]
class ec_errort(Structure):
    pass
ec_errort._anonymous_ = ['_0']
ec_errort._fields_ = [
    ('Time', timeval),
    ('Signal', boolean),
    ('Slave', uint16),
    ('Index', uint16),
    ('SubIdx', uint8),
    ('Etype', ec_err_type),
    ('_0', N9ec_errort4DOT_22E),
]
__all__ = ['__uint16_t', 'ECT_REG_DLSTAT',
           'N9ec_errort4DOT_224DOT_23E', '__int16_t',
           'ECT_COES_TXPDO_RR', 'ec_SoEwrite', 'EC_CMD_FPWR',
           'ECT_SDO_DOWN_INIT', 'ECT_SII_FMMU', 'settimeofday',
           'EC_ERR_TYPE_SDO_ERROR', 'ECT_REG_SM1STAT',
           'ECT_REG_RXERR', 'ECT_SII_REV', 'ECT_MBXT_AOE',
           'EC_BUF_COMPLETE', 'ec_SoEattributet', 'timeval',
           '__uint32_t', 'ECT_SDO_UP_REQ', 'ec_cmdtype', '__time_t',
           'ECT_UNSIGNED32', 'itimerval', 'ECT_GET_OE_RES',
           'ECT_REG_TYPE', 'uint16', 'ECT_REG_PDICTL', 'ECT_MBXT_COE',
           'EC_STATE_PRE_OP', '__qaddr_t', 'ECT_SDO_ABORT',
           '__u_short', '__loff_t', 'ECT_SOE_NOTIFICATION',
           'ECT_SDO_DOWN_INIT_CA', 'ECT_SOE_EMERGENCY',
           'ECT_GET_ODLIST_REQ', '__int32_t', 'ECT_COES_SDOREQ',
           'ECT_REG_ESCSUP', '__itimer_which_t', 'ec_SoEnamet',
           'ECT_DOMAIN', 'ECT_REG_DCTIMEFILT', 'ECT_REG_SM1',
           'ECT_SII_SM', 'ECT_REG_SM3', 'ECT_GET_OE_REQ',
           'ECT_SII_MBXPROTO', 'ECT_REG_IRQMASK', 'sigset_t',
           'ECT_MBXT_EOE', 'ECT_REG_FMMU0', 'ECT_REG_FMMU1',
           'ECT_REG_FMMU2', 'ECT_REG_FMMU3', 'ECT_GET_OD_RES',
           'ec_datatype', 'ECT_GET_OD_REQ', 'boolean',
           'ECT_REG_EEPADR', '__ssize_t', 'ECT_REG_SM1ACT',
           '__nlink_t', 'ECT_REAL32', 'ECT_REG_DCSOF',
           'EC_ERR_NO_SLAVES', 'futimes', '__uint64_t', 'fd_set',
           'ECT_BOOLEAN', 'ECT_REG_DCSYSDIFF', '__off64_t',
           'ECT_REG_STADR', '__fd_mask', 'int16', 'EC_ECMD_NOP',
           'ec_bufT', 'EC_ERR_TYPE_SOE_ERROR', 'ECT_REG_DLCTL',
           'N11ec_SoElistt4DOT_26E', 'ECT_SII_MANUF', 'getitimer',
           '__sigset_t', '__clockid_t', '__useconds_t',
           'ECT_UNSIGNED16', 'adjtime', 'ECT_COES_TXPDO', '__timer_t',
           'ECT_REG_DCCUC', 'EC_ERR_TYPE_FOE_ERROR',
           'ECT_SII_TXMBXADR', 'select', 'ECT_INTEGER8',
           'ec_bufstate', 'uint32', 'ECT_REG_DCSYSTIME',
           'EC_ERR_ALREADY_INITIALIZED', 'EC_CMD_BRW', 'ec_err_type',
           'ECT_SII_BOOTRXMBX', 'ECT_INTEGER64', 'ECT_VISIBLE_STRING',
           'EC_CMD_LRD', 'ECT_FOE_BUSY', 'EC_CMD_BRD',
           'ECT_REG_EEPSTAT', 'ec_SoEmappingt', '__blkcnt64_t',
           'ECT_SII_RXMBXADR', '__u_long', 'EC_CMD_LRW',
           'EC_ERR_TYPE_PACKET_ERROR', 'EC_STATE_ACK',
           'EC_STATE_INIT', '__fsfilcnt64_t', 'EC_CMD_APRD',
           '__blkcnt_t', 'ECT_BIT1', 'ECT_UNSIGNED64', 'ECT_BIT3',
           'ECT_BIT4', 'ECT_BIT5', 'ECT_BIT6', 'ECT_BIT7', 'ECT_BIT8',
           '__ino_t', 'ECT_REG_SM2', '__rlim64_t', 'EC_ERR_OK',
           'EC_CMD_APRW', 'timespec', 'EC_CMD_NOP', 'ECT_SII_ID',
           'setitimer', 'EC_BUF_RCVD', 'ECT_FOE_WRITE',
           'EC_ERR_NOT_INITIALIZED', '__mode_t', 'futimesat',
           'ECT_SII_STRING', 'ECT_INTEGER16', 'ECT_REG_EEPDAT',
           '__blksize_t', 'ECT_SII_BOOTTXMBX', 'ECT_COES_EMERGENCY',
           'N9ec_errort4DOT_22E', '__off_t', '__timezone_ptr_t',
           'ECT_REG_ALIAS', '__intptr_t', 'EC_ECMD_RELOAD',
           'ECT_REG_SM1CONTR', 'EC_ERR_TYPE_EMERGENCY', 'ec_state',
           'ECT_REG_SM0', 'ECT_REG_EEPCTL', 'ECT_SDO_SEG_UP_REQ',
           'EC_ERR_NOK', '__suseconds_t', 'ECT_SII_GENERAL',
           'ECT_FOE_ERROR', 'EC_CMD_APWR', 'ECT_REG_ALCTL',
           'EC_STATE_SAFE_OP', 'ECT_SII_PDO', 'ECT_SOE_READREQ',
           'ECT_SOE_READRES', 'EC_BUF_EMPTY', '__uint8_t',
           'ECT_REAL64', 'ITIMER_REAL', '__u_char', '__rlim_t',
           'uint8', '__sig_atomic_t', 'gettimeofday', 'EC_CMD_FPRD',
           'ECT_REG_ALSTAT', 'ec_SoElistt', 'ECT_TIME_DIFFERENCE',
           'pselect', 'int8', 'ECT_SDOINFO_ERROR', 'uint64',
           'ECT_REG_DLALIAS', 'EC_ERR_TYPE_FOE_BUF2SMALL',
           'ECT_FOE_READ', 'EC_ERR_TIMEOUT', 'ECT_REG_DLPORT',
           'ECT_MBXT_ERR', 'EC_BUF_TX', 'ECT_SDO_UP_REQ_CA',
           'ECT_FOE_ACK', '__socklen_t', 'ECT_REG_DCSYSDELAY',
           'fd_mask', 'ECT_FOE_DATA', 'EC_CMD_FPRW', 'ECT_INTEGER32',
           '__ino64_t', 'ECT_REG_EEPCFG', 'ec_etherheadert',
           'ECT_UNICODE_STRING', 'EC_CMD_BWR', 'EC_BUF_ALLOC',
           '__caddr_t', 'ECT_REG_PORTDES', 'ECT_TIME_OF_DAY',
           '__u_int', '__clock_t', 'timezone', '__fsblkcnt_t',
           'ECT_GET_ODLIST_RES', 'ECT_UNSIGNED8', 'ECT_REG_DCSYNCACT',
           'ECT_REG_DCSYSOFFSET', 'time_t', 'EC_CMD_FRMW',
           'EC_STATE_BOOT', 'EC_CMD_ARMW', 'EC_STATE_OPERATIONAL',
           'ECT_MBXT_SOE', 'ECT_COES_RXPDO_RR', '__gid_t', '__fsid_t',
           'ECT_BIT2', 'ECT_REG_DCCYCLE1', 'ECT_REG_DCCYCLE0',
           '__u_quad_t', 'ECT_COES_SDORES', 'ECT_COES_RXPDO',
           'lutimes', 'EC_ECMD_WRITE', '__id_t', 'EC_ECMD_READ',
           'ec_comt', '__itimer_which', '__pid_t', 'ECT_INTEGER24',
           'ec_readIDNmap', 'ECT_REG_SM0STAT', 'ECT_SOE_WRITERES',
           '__fsblkcnt64_t', 'ECT_SOE_WRITEREQ', 'ec_SoEread',
           'int32', 'ECT_SII_MBXSIZE', 'ECT_REG_DCTIME1',
           'ECT_REG_DCTIME0', 'ECT_REG_DCTIME3', 'ECT_REG_DCTIME2',
           'EC_ERR_TYPE_SDOINFO_ERROR', 'ECT_MBXT_VOE',
           'ECT_REG_DCSPEEDCNT', 'ECT_REG_ALSTATCODE', '__key_t',
           '__swblk_t', 'ITIMER_PROF', 'ec_err', 'ECT_REG_DCSTART0',
           'EC_CMD_LWR', 'ITIMER_VIRTUAL', 'EC_STATE_ERROR',
           'ECT_MBXT_FOE', 'ECT_OCTET_STRING', '__fsfilcnt_t',
           'ECT_UNSIGNED24', 'ec_errort', '__quad_t', '__int64_t',
           'int64', 'ECT_COES_SDOINFO', 'ec_ecmdtype',
           'EC_ERR_TYPE_FOE_PACKETNUMBER', '__daddr_t', '__uid_t',
           'utimes', '__int8_t', 'suseconds_t', '__dev_t']
