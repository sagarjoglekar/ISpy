package ispy.main;


import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;

public class Preview extends SurfaceView implements SurfaceHolder.Callback {
	 private static final String TAG = "Preview";

	
	  SurfaceHolder mHolder;  // <2>
	  public Camera camera; // <3>
	  int width, height;
	  private OpenCV opencv = new OpenCV();
	  int [] image;
	  int [] face_coord;
	  int i = 0;
	  BitmapDrawable face;
	  FileOutputStream fOut = null;
	  String path = "/sdcard";
	  int frames=0;
	  int detected=0;
	 
	  //final ImageView frameview=
	  int [] pixels;
	  //Canvas c;
	  Preview(Context context) {
	    super(context);

	    // Install a SurfaceHolder.Callback so we get notified when the
	    // underlying surface is created and destroyed.
	    mHolder = getHolder();  // <4>
	    mHolder.addCallback(this);  // <5>
	    mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); // <6>
	  }

	  // Called once the holder is ready
	  public void surfaceCreated(SurfaceHolder holder) {  // <7>
	    // The Surface has been created, acquire the camera and tell it where
	    // to draw.
		
	    camera = Camera.open(); // <8>
	   /* Camera.Parameters parameters = camera.getParameters();
		parameters.setPreviewSize(320, 440); 
	    camera.setParameters(parameters);*/
	    try {
	      camera.setPreviewDisplay(holder);  // <9>

	      camera.setPreviewCallback(new PreviewCallback() { // <10>
	        // Called for each frame previewed
	        public void onPreviewFrame(byte[] data, Camera camera) {
	        	frames++;
	        	long start = System.currentTimeMillis();
	        	if(frames==5){
	        	image = new int[width*height];
				decodeYUV(image, data, width,height);
				//Bitmap bitmap = Bitmap.createBitmap(image,width, height, Config.ARGB_8888);
				
				Log.e("notify","Widthe = "+ width +"heighte=" + height);
			    opencv.setSourceImage(image, width, height);
			    int cord,x,y,r;
			    cord=opencv.facedetect();
			    global.Cx=opencv.getX();
			    global.Cy=opencv.getY();
			    global.R=opencv.getR();

			    if(cord>=1){
			    	if(detected==0){
			    			    Log.d(TAG, "Face Coordinates " + Integer.toString(cord)+" "+Integer.toString(global.Cx)+" "+Integer.toString(global.Cy)+" "+Integer.toString(global.R));
			    data = opencv.getSourceImage();
			    Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);
			    
			    
			    FileOutputStream outstream;
				try {
					outstream = new FileOutputStream("/sdcard/face"+Integer.toString(i)+".jpg");
					i++;
					//outstream.write(data);
					bitmap.compress(Bitmap.CompressFormat.JPEG, 100, outstream);
				    outstream.close();
				    long end = System.currentTimeMillis();
					long elapse = end - start;
				    Send_email();
			          Log.d("EMAIL", "EMAIL SENT: " + System.currentTimeMillis() + " " + elapse);

				} catch (FileNotFoundException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				detected=1;
			    	}
			    }
			   frames=0;
			   if(global.onlydetect==1){
			    	if(detected==1){
			    		camera.stopPreview();
			    	}
			    	
			    }
			    
	        	}
			    
			   
				

				 long end = System.currentTimeMillis();
				 long elapse = end - start;  
	          Log.d(TAG, "********************************onPreviewFrame called at: " + System.currentTimeMillis() + " " + elapse);
	         
	          Preview.this.invalidate();  // <12>
	        }
	      });
	    } catch (IOException e) { // <13>
	      e.printStackTrace();
	    }
	    
	    
	  }//00912024203077

	  // Called when the holder is destroyed
	  public void surfaceDestroyed(SurfaceHolder holder) {  // <14>
	    camera.stopPreview();
	    camera = null;
	  }

	  // Called when holder has changed
	  public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		  //Camera.Parameters p = camera.getParameters(); 
          //p.setPreviewSize(380, 340); 
          //camera.setParameters(p); 
			//parameters.setZoom(0);
			//parameters.setRotation(0);
			
			
			camera.startPreview();
	     }

	  @Override
	    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
	        // We purposely disregard child measurements because act as a
	        // wrapper to a SurfaceView that centers the camera preview instead
	        // of stretching it.
	         width = resolveSize(getSuggestedMinimumWidth(), widthMeasureSpec);
	         height = resolveSize(getSuggestedMinimumHeight(), heightMeasureSpec);
	        setMeasuredDimension(width, height);

	        Log.e("Measured", "Height = "+height+" width="+ width);
	    /*    if (mSupportedPreviewSizes != null) {
	            mPreviewSize = getOptimalPreviewSize(mSupportedPreviewSizes, width, height);
	        }*/
	    }
	  
	  public void decodeYUV(int[] out, byte[] fg, int width, int height)
      throws NullPointerException, IllegalArgumentException {
  int sz = width * height;
  if (out == null)
      throw new NullPointerException("buffer out is null");
  if (out.length < sz)
      throw new IllegalArgumentException("buffer out size " + out.length
              + " < minimum " + sz);
  if (fg == null)
      throw new NullPointerException("buffer 'fg' is null");
  if (fg.length < sz)
      throw new IllegalArgumentException("buffer fg size " + fg.length
              + " < minimum " + sz * 3 / 2);
  int i, j;
  int Y, Cr = 0, Cb = 0;
  for (j = 0; j < height; j++) {
      int pixPtr = j * width;
      final int jDiv2 = j >> 1;
      for (i = 0; i < width; i++) {
          Y = fg[pixPtr];
          if (Y < 0)
              Y += 255;
          if ((i & 0x1) != 1) {
              final int cOff = sz + jDiv2 * width + (i >> 1) * 2;
              Cb = fg[cOff];
              if (Cb < 0)
                  Cb += 127;
              else
                  Cb -= 128;
              Cr = fg[cOff + 1];
              if (Cr < 0)
                  Cr += 127;
              else
                  Cr -= 128;
          }
          int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
          if (R < 0)
              R = 0;
          else if (R > 255)
              R = 255;
          int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1)
                  + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
          if (G < 0)
              G = 0;
          else if (G > 255)
              G = 255;
          int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
          if (B < 0)
              B = 0;
          else if (B > 255)
              B = 255;
          out[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
      }
  }

}
	    
	  
	 /* public void loadTile(Drawable tile) {
		  
	        Bitmap bitmap = Bitmap.createBitmap(100, 100, true);
	 
	        Canvas canvas = new Canvas(bitmap);
	 
	        tile.setBounds(0, 0, 100, 100);
	 
	        tile.draw(canvas);
	 
	        mBitmap = bitmap;
	 
	        mCanvas = canvas;
	 
	    }*/
	  @Override
		public void draw(Canvas canvas) {
			super.draw(canvas);
			Paint p = new Paint(Color.RED);
			Log.d(TAG, "draw");
			canvas.drawText("PREVIEW", canvas.getWidth() / 2,
					canvas.getHeight() / 2, p);
		}
	  private void makeInt(byte[] img_fl, int[] img)
		{
			int img_tmp, n, m;
			int N=height;
			int M=width;
			
			for(n=0; n<N; n++)
				for(m=0; m<M; m++)
				{
					//only show 8 MSB
					img_tmp = (int)(img_fl[n*M+m]);
					
					img[n*M+m] = (img_tmp );
				}	
			
		}	 

	  public void Send_email()
	  {
		  Mail m = new Mail("rpi26588@gmail.com", "goleta123"); 

	      //String[] toArr = {"ispy.ece594@gmail.com"}; 
	String[] toArr = global.addr_email;
	Log.d("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Email",toArr[0]);
	m.setTo(toArr);
	m.setFrom("rpi26588@gmail.com"); 
	m.setSubject("Privacy Breached!!"); 
	m.setBody("This person has breached the perimeter.");

	try 
	{ 
		m.addAttachment("/sdcard/face"+Integer.toString(i-1)+".jpg"); 

		if(m.send()) 
		{ 
			Log.e("EMAIL","EMAIL SENT"); 
		} 
		else 
		{ 
			Log.e("EMAIL","NOT SENT"); 
		} 
	} 
	catch(Exception e) 
	{ 
		//Toast.makeText(MailApp.this, "There was a problem sending the email.", Toast.LENGTH_LONG).show(); 
		Log.e("MailApp", "Could not send email", e); 
	} 
} 
	  
}
