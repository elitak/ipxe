/*
 * Copyright (C) 2013 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

FILE_LICENCE ( GPL2_OR_LATER );

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ipxe/pinger.h>
#include <ipxe/monojob.h>
#include <ipxe/timer.h>
#include <usr/pingmgmt.h>

/** @file
 *
 * ICMP ping management
 *
 */

/**
 * Display ping result
 *
 * @v src		Source socket address
 * @v sequence		Sequence number
 * @v len		Payload length
 * @v rc		Status code
 */
static void ping_callback ( struct sockaddr *peer, unsigned int sequence,
			    size_t len, int rc ) {

	/* Display ping response */
	printf ( "%zd bytes from %s: seq=%d",
		 len, sock_ntoa ( peer ), sequence );
	if ( rc != 0 )
		printf ( ": %s", strerror ( rc ) );
	printf ( "\n" );
}

/**
 * Ping a host
 *
 * @v hostname		Hostname
 * @v timeout_ms	Timeout between pings, in ms
 * @v len		Payload length
 * @ret rc		Return status code
 */
int ping ( const char *hostname, unsigned long timeout_ms, size_t len ) {
	int rc;

	/* Create pinger */
	if ( ( rc = create_pinger ( &monojob, hostname,
				    ( ( timeout_ms * TICKS_PER_SEC ) / 1000 ),
				    len, ping_callback ) ) != 0 ) {
		printf ( "Could not start ping: %s\n", strerror ( rc ) );
		return rc;
	}

	/* Wait for ping to complete */
	if ( ( rc = monojob_wait ( NULL ) ) != 0 ) {
		printf ( "Finished: %s\n", strerror ( rc ) );
		return rc;
	}

	return 0;
}
