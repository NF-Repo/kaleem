<?php
session_start();
require_once("twitteroauth/twitteroauth.php"); //Path to twitteroauth library
 
$twitteruser = "waterwayscruise";
$notweets = 30;
$consumerkey = "dhJtn5YLJGtl6vy798NQw";
$consumersecret = "UB65eNkSScfORJVZURaGwLMnGoFZ6x8hxt3bxNVT4M";
$accesstoken = "1168223072-S9YYOi2jzij6umuWCKrkmxX2GFjlV5RKq4k0zOK";
$accesstokensecret = "ZzVHpsljfAXlb1bEHTO1bGWVGzoaxrI3gb2wXdBeXJk";
 
function getConnectionWithAccessToken($cons_key, $cons_secret, $oauth_token, $oauth_token_secret) {
  $connection = new TwitterOAuth($cons_key, $cons_secret, $oauth_token, $oauth_token_secret);
  return $connection;
}
 
$connection = getConnectionWithAccessToken($consumerkey, $consumersecret, $accesstoken, $accesstokensecret);
 
$tweets = $connection->get("https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=".$twitteruser."&count=".$notweets);
foreach ($tweets as $tweettext) {
  var_dump($tweettext->text);
}


//var_dump($tweets);
//echo json_encode($tweets);
?>