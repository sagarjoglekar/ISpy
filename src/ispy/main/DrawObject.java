package ispy.main;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.view.View;

public class DrawObject extends View{

	public DrawObject(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	protected void onDraw(Canvas canvas){
		System.err.println("DetectView: Draw");
		int i;
		Paint myPaint = new Paint();
		myPaint.setColor(Color.RED);
		myPaint.setStyle(Style.STROKE);
		myPaint.setStrokeWidth(3f);
		canvas.drawCircle(global.Cx+global.xoffset, global.Cy+global.yoffset, global.R, myPaint);
		//canvas.drawArc(oval, startAngle, sweepAngle, useCenter, paint)
	}
}
