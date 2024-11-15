<?php


class templateFromFile extends template{
	public function __construct(string $file,bool $useRoot=false) {
		
		
		if(!$useRoot){
			$script=explode('/',$_SERVER['SCRIPT_NAME']);
			if(empty($script[1]) or $script[1]=='index.php')$script[1]='';
		}else{
			$script[1]='';
		}
		$path=$_SERVER['DOCUMENT_ROOT']."/".$script[1].'/'.$file;
		if (!file_exists($path)) {
			return $this->error('File does not exist: '.$path);
		}
		$this->string = file_get_contents($path);
    }

	public function setArr($dataArr,?string $customDateFormat=NULL):void{
		parent::setArr($dataArr,$customDateFormat);
	}

	public function set(string $key,?string $value):void {
		parent::set($key,$value);
	}

	public function draw():string{
		return parent::draw();
	}

	public function dateFormat(?string $customDateFormat):void{
		parent::dateFormat($customDateFormat);
	}
}
