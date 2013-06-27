package ispy.main;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class email extends Activity 
{
	/** Called when the activity is first created. */
     EditText addr;
     String user; 

	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		
		super.onCreate(savedInstanceState); 
		setContentView(R.layout.email); 
		addr = (EditText) findViewById(R.id.emailaddress);
		Button email_ok = (Button) findViewById(R.id.email_ok);
//		addr.setText("set email address");

		
		 
		email_ok.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				user = addr.getText().toString();  
//				String toArr[] = {addr.getText().toString()};
					global.addr_email[0] = user.trim();
//				

				Log.d("value", "email" + global.addr_email[0]);
				Log.d("Intent 1", "EXIT");

				//				finish();

			} 
		});

		Button email_back = (Button) findViewById(R.id.email_back); 
		email_back.setOnClickListener(new View.OnClickListener() 
		{ 
			public void onClick(View view) 
			{ 
				finish();

			} 
		});
	}
}