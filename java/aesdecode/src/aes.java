import javax.crypto.*;
import javax.crypto.spec.*;
import java.security.spec.*;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

public class aes {

	/* BEGIN code from Interface Kft */
	private String initVector = "tsrvector956748245542"; // In practice, it's only "tsrvector9567482", since only the first 16 chars are used
	private String salt = "akdirn%/=(+\"922xc&@s";
	private Integer pswdIterations = 128;
	private String password = "_D?+!!%/\"weg112W";

	public byte[] aesDeCode( byte[] in ) throws Exception {

		byte[] saltB=salt.getBytes("UTF-8");
		
		byte[] initVectorB =new byte[16];
		
		byte[] wB=initVector.getBytes("UTF-8");
		
		for (int i = 0; i < 16; i++) {
			if (i<wB.length) initVectorB[i]=wB[i];
			else initVectorB[i]=(byte) (-100-i);
		}
		
		if (pswdIterations==null) pswdIterations=1024;

		SecretKeyFactory factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
		KeySpec spec = new PBEKeySpec(password.toCharArray() , saltB, pswdIterations, 256);
		SecretKey tmp = factory.generateSecret(spec);
		SecretKey secret = new SecretKeySpec(tmp.getEncoded(), "AES");
		Cipher cipher= Cipher.getInstance("AES/CBC/PKCS5Padding");
		/* Decrypt the message. */
		cipher.init(Cipher.DECRYPT_MODE, secret, new IvParameterSpec(initVectorB));
		return cipher.doFinal(in);
	}
	/* END code from Interface Kft */

	/**
	 * Main function for testing with "javac Foo.java; java Foo"
	 */
	public static void main(String [] args) {
		// File location
		Path path = Paths.get("f:\\downloads\\TSREXPORT_20131217144602");
		try {
			// Read file into memory
			byte[] data = Files.readAllBytes(path);
			// New instance of self
			aes foo = new aes();
			System.out.println("Decoded:");
			// Decode!
			byte[] decoded_raw = foo.aesDeCode(data);
			System.out.println(decoded_raw.length);
			String decoded_str = new String(decoded_raw);
			System.out.println(decoded_str);
		} catch (Exception e) {
			System.out.println("Exception:");
			System.out.println(e);
		}
	}
}
