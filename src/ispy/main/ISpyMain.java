package ispy.main;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class ISpyMain extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
		
		super.onCreate(savedInstanceState); 
		setContentView(R.layout.menu); 
		
	//  Face Detect
		Button detect = (Button) findViewById(R.id.detect); 
		Button track = (Button) findViewById(R.id.track);
		
		detect.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				//
				global.onlydetect=1;
				Intent Detect = new Intent(ISpyMain.this, simplepreview.class );
		        startActivity (Detect);
				
			} 
		});
 
		track.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				//
				Intent Detect_track = new Intent(ISpyMain.this, simplepreview.class );
		        startActivity (Detect_track);
			} 
		});
		
//		Button touch_track = (Button) findViewById(R.id.touch_track); 
//		touch_track.setOnClickListener(new View.OnClickListener() 
//		{ 
//			public void onClick(View view) 
//			{ 
//
//				//
//			} 
//		});
		
		Button email = (Button) findViewById(R.id.email); 
		email.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				Intent myEmail = new Intent(ISpyMain.this, email.class );
		        startActivity (myEmail);
				 
			} 
		});
		
		Button about = (Button) findViewById(R.id.about); 
		about.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				Intent myAbout = new Intent(ISpyMain.this, about.class );
		        startActivity (myAbout);
			} 
		});
	} 
}