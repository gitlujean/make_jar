//
//test jar
//

import watermark.wmLib;

public class test {
		
		
	public static void main(String[] args) {
			
			int x = 300;
			int y = 300;

			String filename = "/home/developer/images/cat.jpg";
			String msg = "12341234";
			int strength = 200000000;
			
			System.out.println("Add " + msg + " in image " + filename) ;
			wmLib.AddWatermark256(filename,
					msg,
					x, y, strength);
			
			String detmsg = wmLib.DetectWatermark256(filename,
					x, y);
			
			System.out.println("Detected " + detmsg);
		}
			
    }			
