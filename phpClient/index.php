<?php
	$url = "http://192.168.1.200/";
	
	if(isset($_POST['type'])){
		$urlToRequest = processFormSubmit($_POST, $url);
	}else{
		$urlToRequest = $url;
	}
	
	$response = file_get_contents($urlToRequest);
	$data = json_decode($response, true);
		
	$temperature = $data["t"] / 100;
	$humidity = $data["h"];
	$toas = $data["toas"];
	$toot = $data["toot"];
	
	$heaterOn = $toas != 0 || $toot != 0;
	
	include('template/head.php');
	include('template/table.php');
	
	if($heaterOn){
		include('template/turnOffModal.php');
	}else{
		include('template/turnOnModal.php');
	}
	
	include('template/footer.php');
	
	// Functions
	
	function processFormSubmit($post, $url){
		$type = $post['type'];
		if($type == "turnOn"){
			$pass = $post['pass'];
			$for = $post['for'];
			$forExplode = explode("-", $for);
			$functionName = ($forExplode[0] == "d") ? "turnOnForDeltaT" : "turnOnLimitedTime";
			$amount = $forExplode[1];
			return $url."?".$functionName."=".$amount.":".$pass;
		}
		
		if($type == "turnOff"){
			$pass = $post['pass'];
			return $url."?turnOff=:".$pass;
		}
		
		return $url;
	}
	
	
?>