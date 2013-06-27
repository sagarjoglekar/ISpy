package ispy.main;

public class OpenCV {
	static{
		System.loadLibrary("opencv");
	}
	public native boolean setSourceImage(int[] pixels, int width, int height);
	public native byte[] getSourceImage();
	public native int extractSURFFeature();
	public native int facedetect();
	public native int getX();
	public native int getY();
	public native int getR();
	public native int trackX();
}
