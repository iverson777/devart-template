package  {
	import flash.display.Sprite;
	
	public class LIGHT extends Sprite{
		private var LED:Sprite;
		private var i:int;
		private var WIDTH:int=100;
		private var HEIGHT:int=100;
		private var COLOR:int=0xFFFFFF;
		public function LIGHT() {
			// constructor code
			LED=new Sprite();
			initGraphic();
			this.addChild(LED);
		}
		private function initGraphic():void{
			LED.graphics.beginFill(COLOR);
			LED.graphics.drawRect(0,0,WIDTH,HEIGHT);
			LED.graphics.endFill();
		}

	}
	
}
