<?php
include('Cart.php');
include('Form.php');

$Booking = new Booking();
//var_dump($_SERVER['REQUEST_METHOD']);
if($_SERVER['REQUEST_METHOD'] == 'GET') {
//  var_dump($_GET);
	$Form = new Form($Booking->form(),$_GET);
       
        
//         { ["cart_id"]=> string(26) "bvk3diir2rnlt4fga48tpv0rr4" ["create"]=> string(10) " Book Now " } 
        $getdata=array(
            'cart_id'=>$_GET['cart_id'],
            'create'=>'Book Now ',
            'customer_name'=>'jhon',
            'customer_email'=>'lakshminarsimha@lsnsoft.com',
            'customer_country'=>'US',
            'customer_region'=>'AK',
            'customer_postal_zip'=>99556
        );
   
	$response = $Booking->create($getdata);
       
	if($response['request']['status'] == 'OK') {
		// successful transactions will return a url to be redirected to for payment or an invoice.
		header("Location: {$response['request']['url']}"); 
		exit;

	} else {
		$Form->msg($response['request']['msg'],$response['request']['status']);
	}
} else {
	$Form = new Form($Booking->form());
//        var_dump($Form);
}
header('Content-type: text/html; charset=utf-8');
?>
<html>
<head>
<style style="text/css">
body { font:90% "Helvetica Neue",Helvetica,Arial,sans-serif; }
label { width: 10em; display: block; text-align: right; font-weight: bold; float: left; margin-right: 1em;}
input, select, textarea { width: 20em; display: block; }
.msg.ERROR { color: firebrick; font-weight: bold;}
</style>
</head>
<body>
<h1>Checkfront Shopping Card Demo</h1>
<form method="post" action="<?=$_SERVER['SCRIPT_NAME']?>?cart_id=<?=$_GET['cart_id']?>">
<fieldset>
<?php
echo $Form->msg();
if(!count($Form->fields)) {
	echo '<p>ERROR: Cannot fetch fields.</p>';
} else {
	foreach($Form->fields as $field_id => $data) {
		if(!empty($data['define']['layout']['lbl'])) {
			echo "<label for='{$field_id}'>" . $data['define']['layout']['lbl'] . ':</label>';
		}
		echo $Form->render($field_id);
		echo '<br />';
	}
	echo '<button type="submit"> Continue </button>';
}
?>
<pre style="margin-left: 10px">
<strong>Debug Information</strong>
Cart ID: <input type="text" readonly="readonly" name="cart_id" value="<?=$Booking->cart_id?>" /> 
</fieldset>
</form>
</body>
</head>
</html>
