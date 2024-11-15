<?php


class material extends template{
	protected $headerUrls='
	<link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">';
	protected $footerUrls='';
	protected $headerStr='';
	protected $footerStr='';
	protected $usesBodyTags=true;

	protected $nightmode=true;

	public function __construct(string $bodyStr='',?string $title=null){
		$_ENV['plain']=false;
		$this->loadTemplates();
		$this->setBody($bodyStr);
		$bodyTag='<body>';
		if($this->nightmode){
			$bodyTag='<body class="nightMode">';
		}
		$title=$title??$_ENV['appName']??'title';
		$this->setArr([
			'body'=>$bodyTag,
			'/body'=>'</body>',
			'header'=>'',
			'footer'=>'',
			'title'=>$title,
			]
		);
	}

	public function setTitle(string $title):void{
		$this->set('title',$title);
	}

	public function setBody(string $bodyStr):void{
		$bodyStr='
<pre style="font-family: monospace;white-space: pre;display:none">
Tʜᴇ ᴄᴀᴋᴇ ᴡɪʟʟ ʙᴇ sᴇʀᴠᴇᴅ ᴀᴛ ᴛʜᴇ ᴄᴏɴᴄʟᴜsɪᴏɴ ᴏғ ᴛʜɪs ᴍᴇᴇᴛɪɴɢ

              .,-:;//;:=,
          . :H@@@MM@M#H/.,+%;,
       ,/X+ +M@@M@MM%=,-%HMMM@X/,
     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-
    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.
  ,%MM@@MH ,@%=             .---=-=:=,.
  =@#@@@MX.,                -%HX$$%%%:;
 =-./@M@M$                   .;@MMMM@MM:
 X@/ -$MM/                    . +MM@@@M$
,@M@H: :@:                    . =X#@@@@-
,@@@MMX, .                    /H- ;@M@M=
.H@@@@M@+,                    %MM+..%#$.
 /MMMM@MMH/.                  XM@MH; =;
  /%+%$XHH@$=              , .H@@@@MX,
   .=--------.           -%H.,@@@@@MX,
   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.
     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=
       =%@M@M#@$-.=$@MM@@@M; %M%=
         ,:+$+-,/H#MMMMMMM@= =,
			  =++%%%%+/:-.
			  
Dᴇᴠᴇʟᴏᴘᴇᴅ ʙʏ Kᴀᴍɪʟ Sᴢʏᴍᴄᴢᴀᴋ ғᴏʀ Iɴᴅɪɢᴏ-Nᴀɪʟs
			  </pre> 
'.$bodyStr;
		$this->set('content',$bodyStr);
	}

	public function useBodyTags(bool $bool=TRUE):void{
		$this->usesBodyTags=$bool;		
	}
	
	public function setToHeader(array $urls):void{
		$this->headerUrls.=$this->iterate($urls);
	}

	public function setToFooter(array $urls):void{
		$this->footerUrls.=$this->iterate($urls);
	}

	public function getHeader():string{
		return $this->populate($this->headerStr);
	}

	public function getFooter():string{
		return $this->populate($this->footerStr);
	}

	public function draw():string{
		return $this->populate($this->headerStr.$this->footerStr);
	}

	private function iterate(array $arr):string{
		$response='';
		foreach($arr as $val){
			$response.=$val;
		}
		return $response;
	}

	private function populate($string):string{
		if(!$this->usesBodyTags){
			$this->setArr(['body'=>'','/body'=>'']);
		}

		if($this->headerUrls)$this->set('header',$this->headerUrls);
		if($this->footerUrls)$this->set('footer',$this->footerUrls);
		$output=str_replace(array_keys($this->values), $this->values, $string);
		return $output;
	}

	private function loadTemplates():void{
		$this->headerStr=file_get_contents('/includes/html/material/header.html');
		$this->footerStr=file_get_contents('/includes/html/material/footer.html');
	}

	

}