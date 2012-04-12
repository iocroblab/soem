# -*- coding: utf8 -*-
import ethercattype as ecty
import ethercatconfig as ecfg
import ethercatmain as ecmn
import ethercatcoe as ecoe
import ctypes as ct
import time

# Definició de variables globals
#IOmap = ct.c_uint8(0)*4096
IOmap = ct.c_buffer(b'\0'*4096)

# Definició de constants:
EC_STATE_SAFE_OP = ecty.EC_STATE_SAFE_OP
#  Aquestes constants estan definides a ethercattype.h, 
#  però per algun obscur motiu, no les pytonitza 
EC_TIMEOUTSTATE = 2000000   # ecty.EC_TIMEOUTSTATE

# Mapatge de variables de la libsoem
ec_slavecount = ecmn.ec_slavecount.value
ec_slave = ecmn.ec_slave
ec_group = ecmn.ec_group

# Mapatge de funcions
#
ec_init = ecmn.ec_init
ec_close = ecmn.ec_close
#
ec_config_init = ecfg.ec_config_init
ec_config_map = ecfg.ec_config_map
# ec_config = ec_config_init + ec_config_map
ec_config = ecfg.ec_config
#
ec_statecheck = ecmn.ec_statecheck

# El programa comença aquí

#time.sleep(2)
ec_init("eth1")
slaves = ec_config_init(0)
print("S'han trobat {0} slaves".format(slaves))
wkc = ec_config_map(IOmap)
print("El workcounter és: {0}".format(wkc))
'''
  # print("Workcounter calculat: {0}".format(ec_group[0].expectedWKC))
  ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 3)
'''
ec_close()
