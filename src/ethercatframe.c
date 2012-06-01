/*
 * Simple Open EtherCAT Master Library 
 *
 * File    : ethercatframe.c
 * Version : 1.2.6
 * Date    : 25-07-2011
 * Copyright (C) 2005-2011 Speciaal Machinefabriek Ketels v.o.f.
 * Copyright (C) 2005-2011 Arthur Ketels
 * Copyright (C) 2008-2009 TU/e Technische Universiteit Eindhoven 
 *
 * SOEM is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * SOEM is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * As a special exception, if other files instantiate templates or use macros
 * or inline functions from this file, or you compile this file and link it
 * with other works to produce a work based on this file, this file does not
 * by itself cause the resulting work to be covered by the GNU General Public
 * License. However the source code for this file must still be made available
 * in accordance with section (3) of the GNU General Public License.
 *
 * This exception does not invalidate any other reasons why a work based on
 * this file might be covered by the GNU General Public License.
 *
 * The EtherCAT Technology, the trade name and logo “EtherCAT” are the intellectual
 * property of, and protected by Beckhoff Automation GmbH. You can use SOEM for
 * the sole purpose of creating, using and/or selling or otherwise distributing
 * an EtherCAT network master provided that an EtherCAT Master License is obtained
 * from Beckhoff Automation GmbH.
 *
 * In case you did not receive a copy of the EtherCAT Master License along with
 * SOEM write to Beckhoff Automation GmbH, Eiserstraße 5, D-33415 Verl, Germany
 * (www.beckhoff.com).
 */

/** \file
 * \brief
 * EtherCAT frame and datagram functions. 
 */

#include <stdio.h>
#include <string.h>
#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"

/** ethernet header definition */
typedef struct PACKED 
{
	/** destination MAC */
	uint16  da0,da1,da2;
	/** source MAC */
	uint16  sa0,sa1,sa2;
	/** ethernet type */
	uint16  etype;
} ec_etherh_t;

/** ethernet header size */
#define ETH_HEADERSIZE		sizeof(ec_etherheadert)

/** EtherCAT header definition */
typedef struct PACKED
{
	/** length of EtherCAT packet */
	uint16	elength:11;
	uint16	res:1;
	uint16	ecattype:4;
} ec_ecath_t;

/** EtherCAT datagram header definition */
typedef struct PACKED
{
	/** EtherCAT command, see ec_cmdtype */
	uint8   command;
	/** index, used in SOEM for Tx to Rx recombination */
	uint8   index;
	/** ADP */
	uint16  ADP;
	/** ADO */
	uint16  ADO;
	/** length of data portion in datagram */
	uint16  dlength:11;
	uint16	r:3;
	uint16	c:1;
	uint16	m:1;
	/** EtherCAT event request */
	uint16  eer;
} ec_ecatd_t;

#define EC_MAXDATAGRAM	115

typedef struct
{
	uint16		start;
	uint16		wkc;
} ec_dinfo_t;

typedef struct
{
	int			txstat;
	int			rxstat;
	void		*txframe;
	void		*rxframe;
	int			headersize;
	ec_dinfo_t	datagram[EC_MAXDATAGRAM];
	uint8		ndatagram;
	uint8		index;
} ec_packetinfo_t;



