<?php
// define static var
//define('DRUPAL_ROOT', getcwd());
// include bootstrap
include_once('./includes/bootstrap.inc');
// initialize stuff
//drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class Checkfront extends CheckfrontAPI {
/*public $tmp_file = '.checkfront_oauth';*/
  public function __construct($data) {
    parent::__construct($data);
    session_start();
  }

  /* DUMMY Data store.  This sample stores oauth tokens in a text file...
   * This is NOT reccomened in production.  Ideally, this would be in an encryped 
   * database or other secure source.  Never expose the client_secret or access / refresh
   * tokens.
   *
   * store() is called from the parent::CheckfrontAPI when fetching or setting access tokens.  
   *
   * When an array is passed, the store should save and return the access tokens.
   * When no array is passed, it should fetch and return the stored access tokens.
   *
   * param array $data ( access_token, refresh_token, expire_token )
   * return array
   */
final protected function store($data = array()) {

		/*$tmp_file = sys_get_temp_dir() . DIRECTORY_SEPARATOR. $this->tmp_file;*/
    $credentials = entity_load('checkfrontadmin', $ids = FALSE, $conditions = array('id' => 1));
    if (count($data)) {
      $checkfrontObj = new stdClass();
      $checkfrontObj->id = '1';
      $checkfrontObj->accessToken = $data['access_token'];
      $checkfrontObj->tokenupdated = $data['updated'];
      $checkfrontObj->tokenexpire = $data['expire_token'];
      $checkfrontObj->refreshToken = $data['refresh_token'];
     $checkfrontObj->created = time();
      $re = checkfrontadmin_save($checkfrontObj);
     /*watchdog('storecheckfrontfunction', 'newcredentialsave');
	file_put_contents($tmp_file,json_encode($data,true));*/
    }
    elseif (count($credentials) > 0) {

/*$data = json_decode(trim(file_get_contents($tmp_file)),true);
watchdog('storecheckfrontfunction', 'credentialsfound');*/
      foreach ($credentials as $values) {
        $data["refresh_token"] = $values->refreshToken;
        $data["access_token"] = $values->accessToken;
        $data["expire_token"] = $values->tokenexpire;
        $data["updated"] = $values->tokenupdated;
      }
    }

    return $data;
  }

  public function session($session_id, $data = array()) {
    $_SESSION['checkfront']['session_id'] = $session_id;
  }

}

?>