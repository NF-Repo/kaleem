<?php
class _Groupons {
	public function updateGroupons($vouchercodes) {
		
		for($i=1;$i<=count($vouchercodes);$i++){
			if(isset($vouchercodes[$i])){
				if(!empty($vouchercodes[$i]['code'])){
					try {
						$num_updated =
						db_update ( 'deal_grouponcodes' )
						->fields ( array ('is_used' => '1') )
						->condition ( 'groupon_code', $vouchercodes[$i]['code'], '=' )
						->execute ();
					} catch ( Exception $e ) {
					}
				}
			}
		}
	}
}