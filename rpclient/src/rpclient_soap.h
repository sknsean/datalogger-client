/*
 * Energinet Datalogger
 * Copyright (C) 2009 - 2012 LIAB ApS <info@liab.dk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef RPCLIENT_SOAP_H_
#define RPCLIENT_SOAP_H_


struct rpclient_soap {
    const char *address;
    const char *username;
    const char *password;
    const char *authrealm;
    struct soap *soap;
    struct http_da_info *dainfo;
    int dbglev;
};

int rpclient_soap_init(struct rpclient_soap *rpsoap);

void rpclient_soap_deinit(struct rpclient_soap *rpsoap);

int rpclient_soap_hndlerr(struct rpclient_soap *rpsoap,struct soap *soap,  struct http_da_info *info, 
			  int retries, const char *funname);


#endif /* RPCLIENT_SOAP_H_ */
