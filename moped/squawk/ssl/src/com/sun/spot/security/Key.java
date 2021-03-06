/*
 * Copyright 2006-2008 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * only, as published by the Free Software Foundation.
 * 
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included in the LICENSE file that accompanied this code).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 16 Network Circle, Menlo
 * Park, CA 94025 or visit www.sun.com if you need additional
 * information or have any questions.
 */

package com.sun.spot.security;

/**
 * The Key interface is the top-level interface for all keys. It defines
 *  the functionality shared by all key objects. All keys have three 
 *  characteristics: 
 *  <ul>
 *  <li>An Algorithm</li> 
 *  <p>
 *  This is the key algorithm for that key. 
 *  The key algorithm is usually an encryption or asymmetric operation
 *  algorithm (such as DSA or RSA), which will work with those algorithms
 *  and with related algorithms (such as
 *  MD5 with RSA, SHA-1 with RSA, Raw DSA, etc.)
 *  <li>An Encoded Form</li>
 *  <p>
 *  This is an external encoded form for the key used when a standard 
 *  representation of the key the Java Virtual Machine, as when transmitting
 *  the key to some other party. The key is encoded standard format 
 *   (such as <code>X.509 SubjectPublicKeyInfo</code>). Note: The syntax of the ASN.
 *   <code>SubjectPublicKeyInfo</code> is defined as follows:<br>
 *   <code><pre>
 *   	SubjectPublicKeyInfo ::= SEQUENCE {
 *	  algorithm AlgorithmIdentifier,
 *	  subjectPublicKey BIT STRING }
 *	AlgorithmIdentifier ::= SEQUENCE {
 *	  algorithm OBJECT IDENTIFIER,
 *	  parameters ANY DEFINED BY algorithm OPTIONAL }
 *   </pre></code>
 *  <p>
 *  For more information, see RFC 2459: Internet X.509 Public Key 
 *  Infrastructure Certificate and CRL Profile 
 *  (http://www.ietf.org/rfc/rfc2459.txt).
 *  <li>A Format</li>
 *  <p>
 *  This is the name of the format of the encoded key.
 *  </ul>
 *  <p>
 *  Keys are generally obtained through key generators and certificates.
 *  Keys may also be obtained from key specifications (transparent 
 *  representations of the underlying key material) through the use of a key 
 *  factory.
 *  
 */ 
public abstract interface Key {
    /**
     * Returns the name of the algorithm associated with this key. 
     * For example, �DSA� would indicate that this key is a 
     * DSA key. See Appendix A in the Java Cryptography Architecture
     * API Specification & Reference for information about 
     * standard algorithm names
     *
     * @return the name of the algorithm associated with this key.
     */
    String getAlgorithm();
    
    /**
     * Returns the name of the primary encoding format of this key, or null 
     * if this key does not support encoding. The primary encoding format is 
     * named in terms of the appropriate ASN.1 data format, if an ASN.1 
     * specification for this key exists. For example, the name of the ASN.1 
     * data format for public keys is <I>SubjectPublicKeyInfo</I>, as defined
     * by
     * the X.509 standard; in this case, the returned format is
     * <code>"X.509"</code>.
     * Similarly, the name of the ASN.1 data format for private keys is 
     * <I>PrivateKeyInfo</I>, as defined by the PKCS #8 standard; in this
     * case, 
     * the returned format is <code>"PKCS#8"</code>. 
     *
     * @return the primary encoding format of the key.
     */
    String getFormat();
    
    /**
     * Returns the key in its primary encoding format, or null 
     * if this key does not support encoding. 
     *
     * @return the encoded key, or null if the key does not support encoding.
     */
    byte[] getEncoded();
}
