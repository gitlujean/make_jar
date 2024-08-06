package watermark;

public class wmLib {
	
	static {
		System.loadLibrary("HduWM256x86");
		System.loadLibrary("wmLibjni");
	}

	private native int addwm(byte[] imgfile, byte[] msg, int x, int y, int strength);
	private native int detwm(byte[] imgfile, byte[] msg, int x, int y);

	public static int AddWatermark256(String imgfile, 
									  String msg,
									  int x, int y,
								      int strength) {
		wmLib lib = new wmLib();
			
		String filename = imgfile +  "\0";
		String wmmsg = msg + "\0";

		System.out.println("adding watermark " + wmmsg);
			
		return lib.addwm(filename.getBytes(), wmmsg.getBytes(), x, y, strength);
	}

	public static String DetectWatermark256(String imgfile,
											int x, int y) {
		wmLib lib = new wmLib();

		String filename = imgfile + "\0";
		byte[] detmsg = new byte[10];
		
		int ret = lib.detwm(filename.getBytes(), detmsg, x, y);
		String msg = new String(detmsg);
		
		System.out.println("detect watermark " + msg);
		return msg;
	}
			
}			
