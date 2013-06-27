package ispy.main;


import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;

public class simplepreview extends Activity {
	private static final String TAG = "CameraDemo";
	Camera camera;
	Preview preview;
	Button buttonClick;
	DrawObject Dw;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		  ImageView image1 = (ImageView) findViewById(R.id.View01);

		preview = new Preview(this);
		Dw =new DrawObject(this);
		((FrameLayout) findViewById(R.id.preview)).addView(preview);
		((FrameLayout) findViewById(R.id.preview)).addView(Dw);
		 image1.setImageResource(R.drawable.icon);
		//((FrameLayout) findViewById(R.id.face)).addView(preview);
		/* FrameLayout myFrameLayout = (FrameLayout) findViewById(R.id.face);
		 ImageView img_icon = new ImageView(null);*/
		buttonClick = (Button) findViewById(R.id.buttonClick);
		buttonClick.setOnClickListener(new OnClickListener() {
			public void onClick(View v) 
			{
				
				Log.d(TAG, "On CLick!!");
				finish();
				
			}
		});

		Log.d(TAG, "onCreate'd");
	}

	ShutterCallback shutterCallback = new ShutterCallback() {
		public void onShutter() {
			Log.d(TAG, "onShutter'd");
		}
	};

	/** Handles data for raw picture */
	PictureCallback rawCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			Log.d(TAG, "onPictureTaken - raw");
		}
	};

	
	
}