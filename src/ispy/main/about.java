package ispy.main;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class about extends Activity
{
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
		
		super.onCreate(savedInstanceState); 
		setContentView(R.layout.aboutme);
		
		Button about_ok = (Button) findViewById(R.id.about_ok); 
		about_ok.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				finish();

			} 
		});
	}
}
