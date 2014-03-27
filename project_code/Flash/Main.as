package  {
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import com.greensock.TweenMax;
	import flash.utils.setTimeout;
	import flash.utils.clearTimeout;
	import flash.display.Bitmap;
	import com.as3nui.nativeExtensions.air.kinect.Kinect;
	
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;
	import com.as3nui.nativeExtensions.air.kinect.events.DeviceEvent;
	import com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent;
	import com.as3nui.nativeExtensions.air.kinect.constants.CameraResolution;
	import flash.display.BitmapData;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	
	public class Main extends MovieClip {
		
		private var i,j:int;
		private var MAXLED:int=25;
		private var line:int=-1;
		private var MAXWIDTH:int=5;
		private var lightArr:Array;
		private var randInt:int;
		private var tunnelArr:Array=new Array(new Array(0,1,2,3,4,5,9,10,14,15,19,20,21,22,23,24),
										new Array(6,7,8,11,13,16,17,18),new Array(12,null));
		private var tunnelSortArr:Array=new Array(0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5,6,7,8,13,18,17,16,11,12,null);
		
		private var circleDelay:Number=0;
		private var circleTime:Number=1.5;
		private var cirlceGap:Number=7;
		
		private var device:Kinect;
		private var depth:Bitmap;
		private var tweenTimeout ;
		private var CurrentScene:int=1;
		public function Main() {
			// constructor code
			initLED();
			initKinect();
			
			//startBlink();
			//startTunnel();
			//switchKinect(true);
			stage.addEventListener(KeyboardEvent.KEY_DOWN,keydown);
		}
		private function keydown(e:KeyboardEvent ):void{
			switchOFF();
			switch(e.keyCode){
				case 49:
					startBlink();
					break;
				case 50:
					startTunnel();
					break;
				case 51:
					switchKinect(true);
					break;
				case 52:
					tunnelType4();
					break;
			}
		}
		private function switchOFF():void{
			removeEventListener(Event.ENTER_FRAME,blinkEnter);
			clearTimeout(tweenTimeout);
			TweenMax.killAll(true);
			switchKinect(false);
		}
		private function initKinect():void{
			depth = new Bitmap(new BitmapData(640,480,false,0xFF000000));
			addChild(depth);
			depth.visible=false;
			depth.x=stage.stageWidth/2-(640/2);
			depth.y=stage.stageHeight/2-(480/2);
			if (Kinect.isSupported())
			{
				device = Kinect.getDevice();
				device.addEventListener(DeviceEvent.STARTED, connected);
				device.addEventListener(DeviceEvent.STOPPED, disconnected);
				device.addEventListener(CameraImageEvent.DEPTH_IMAGE_UPDATE, depthImage);
				var settings:KinectSettings = new KinectSettings();
				settings.depthEnabled = true;
				settings.depthResolution = CameraResolution.RESOLUTION_640_480;
				device.start(settings);
				trace("initialized");
			}
			else
			{
				trace("can't connect");
			}
		}
		private function connected(evt:DeviceEvent):void
		{
			trace("connected");
		}
		private function disconnected(evt:DeviceEvent):void
		{
			trace("disconnected");
		}
		private function depthImage(evt:CameraImageEvent):void
		{
			var bitmapData:BitmapData = evt.imageData;
			depth.bitmapData = bitmapData;
		}
		private function initLED():void{
			lightArr=new Array();
			for(i=0;i<MAXLED;i++){
				if(i%MAXWIDTH==0){
					line++;
				}
				var light:LIGHT=new LIGHT();
				light.x=(i%MAXWIDTH)*light.width;
				light.y=line*light.height;
				light.alpha=0;
				light.mouseChildren=false;
				light.addEventListener(MouseEvent.CLICK,lightClick);
				this.addChild(light);
				lightArr.push(light);
			}
		}
		private function lightClick(e:MouseEvent):void{
			if(e.target.alpha==1){
				e.target.alpha=0;
			}else{
				e.target.alpha=1;
			}
		}
		private function startBlink():void{
			addEventListener(Event.ENTER_FRAME,blinkEnter);
		}
		private function blinkEnter(e:Event):void{
			
			//for(i=0;i<2;i++){
				randInt=int(Math.random()*lightArr.length);
				TweenMax.to(lightArr[randInt],circleTime/4,{alpha:Math.abs(lightArr[randInt].alpha-1),onComplete:fadeOut,onCompleteParams:[lightArr[randInt]]});
				
				//lightArr[randInt].alpha=Math.abs(lightArr[randInt].alpha-1);
			//}
		}
		private function startTunnel():void{
			//tunnelType1();
			//tunnelType2();
			tunnelType3();
			//tunnelType4();
			//tunnelType5();
		}
		private function switchKinect(vis:Boolean):void{
			depth.visible=vis;
		}
		private function tunnelType1():void{
			for(i=0;i<tunnelArr.length;i++){
				for(j=0;j<tunnelArr[i].length;j++){
					if(tunnelArr[i][j]!=null){
						TweenMax.to(lightArr[tunnelArr[i][j]],circleTime,{delay:circleDelay+i/cirlceGap,alpha:1});
					}
				}
			}
			tweenTimeout=setTimeout(tunnelType12,(circleTime+tunnelArr.length/cirlceGap)*1000);
		}
		private function tunnelType12():void{
			for(i=0;i<tunnelArr.length;i++){
				for(j=0;j<tunnelArr[i].length;j++){
					if(tunnelArr[i][j]!=null){
						TweenMax.to(lightArr[tunnelArr[i][j]],circleTime,{delay:circleDelay+i/cirlceGap,alpha:0});
					}
				}
			}
			tweenTimeout=setTimeout(tunnelType1,(circleTime+tunnelArr.length/cirlceGap)*1000);
		}
		private function tunnelType2():void{
			for(i=0;i<tunnelArr.length;i++){
				for(j=0;j<tunnelArr[i].length;j++){
					if(tunnelArr[i][j]!=null){
						TweenMax.to(lightArr[tunnelArr[i][j]],circleTime/2,{delay:circleDelay+i/cirlceGap,alpha:1,onComplete:fadeOut,onCompleteParams:[lightArr[tunnelArr[i][j]]]});
					}
				}
			}
			tweenTimeout=setTimeout(tunnelType2,(circleTime+tunnelArr.length/cirlceGap)*1000);
		}
		private function tunnelType3():void{
			for(i=tunnelArr.length-1;i>=0;i--){
				for(j=0;j<tunnelArr[i].length;j++){
					if(tunnelArr[i][j]!=null){
						TweenMax.to(lightArr[tunnelArr[i][j]],circleTime/2,{delay:circleDelay+(tunnelArr.length-i)/cirlceGap,alpha:1,onComplete:fadeOut,onCompleteParams:[lightArr[tunnelArr[i][j]]]});
					}
				}
			}
			tweenTimeout=setTimeout(tunnelType3,(circleTime+tunnelArr.length/cirlceGap)*1000);
		}
		private function tunnelType4():void{
			for(i=0;i<tunnelSortArr.length;i++){
				if(tunnelSortArr[i]!=null){
					TweenMax.to(lightArr[tunnelSortArr[i]],circleTime/4,{delay:circleDelay+i/cirlceGap/2,alpha:1,onComplete:fadeOut,onCompleteParams:[lightArr[tunnelSortArr[i]]]});
				}
			}
			tweenTimeout=setTimeout(tunnelType5,2.1*1000);
		}
		private function tunnelType5():void{
			for(i=tunnelSortArr.length-1;i>=0;i--){
				if(tunnelSortArr[i]!=null){
					TweenMax.to(lightArr[tunnelSortArr[i]],circleTime/4,{delay:circleDelay+(tunnelSortArr.length-i)/cirlceGap/2,alpha:1,onComplete:fadeOut,onCompleteParams:[lightArr[tunnelSortArr[i]]]});
				}
			}
			tweenTimeout=setTimeout(tunnelType4,2.1*1000);
		}
		private function fadeOut(mc:LIGHT):void{
			TweenMax.to(mc,circleTime/2,{alpha:0});
		}
		
	}
	
}
