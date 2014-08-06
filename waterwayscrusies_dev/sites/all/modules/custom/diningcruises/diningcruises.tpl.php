<?php
?>

<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet"	href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css"	type="text/css" />


<script type="text/javascript">

function cruiseDetails(nid,aliaspath) {

	if(aliaspath=='false'){
    window.location.href = '<?php echo base_path(); ?>cruisesdetailview/category/' + nid;
	}

	else{
		window.location.href = '<?php echo base_path(); ?>'+aliaspath;

}


}

function loadbxslider()
{
    <?php
			$data = CruiseCategory::getCruiseListByGroup ();
		$cnt = count ( $data );
		for($i = 0; $i < $cnt; $i ++) {
		?>
		var i=<?php echo $i; ?>;
    		 $('.slider'+i).bxSlider({
                    infiniteLoop: false,
                    hideControlOnEnd: true,
                     minSlides: 1,
                    maxSlides: 3,
                    slideWidth: 300,
                    slideHeight: 300,
                    slideMargin: 10,
                    touchEnabled: true,
                    pager: true
                });

	  
	  	<?php
			}
		?>
}




$(document).ready(function(){
    
		  loadbxslider();
//    $( window ).resize(function() {
//  loadbxslider();
//});

		
});

	$(window).load(function() {

    	// menu active script

	    // To do active Tab
	    $(".responsiveWeb li").removeClass("active");
	    $('.firstLink').addClass("active");


	      //if (window.location.hash) {
	          //document.location.href = "#featuredCruises";
	      //}
	      $('.cruiseList li .cruiseboxFirst').mouseover(function(e) {
	          $(this).children(".cruiseboxFirstout").hide();
	          $(this).children(".cruiseboxFirstover").show();
	      });
	
	      $('.cruiseList li .cruiseboxFirst').mouseout(function(e) {
	          $(this).children(".cruiseboxFirstout").show();
	          $(this).children(".cruiseboxFirstover").hide();
	      });
	    

	    $('#SunsetBoxFirst').mouseover(function(e) {
            $('#contentboxSecondout').hide();
            $('#contentboxSecondhover').show();
        });
        $('#SunsetBoxFirst').mouseout(function(e) {
            $('#contentboxSecondout').show();
            $('#contentboxSecondhover').hide();

        });
	   var hashreferral = window.location.hash ;
        window.location.href = hashreferral;
   

	});

</script>

<div class="container">
    <div class="contentbox-inner" id="bxsliderarrows">

		<?php
		$data = CruiseCategory::getCruiseListByGroup ();
              
		$i = 0;
		$c = 1;
		foreach ( $data as $key => $values ) {
		$title = CruiseCategory::getCategoryName ( $key );
          
		?>
		<div class="row-fluid" style="margin-top: 10px;">
            <div class="span12">
                <div class="diningTitle"><a name="referrel-<?php echo $c; ?>"><?php echo $title; ?></a></div>
            </div>
        </div>
		<div class="tileimages">
		<div class="slider<?php echo $i; ?>">
		
		<?php
			$numofrecords = count ( $values );
//                        var_dump($numofrecords);
			for($j = 0; $j < $numofrecords; $j ++) {
				
				$cpath = '';
				$path = file_uri_path ( $values [$j]->tile_img_fid );
				if ($path != '') {
					$cpath = base_path () . 'sites/default/files/cruisetemplates/' . $path;
				}else{
					$cpath = base_path () . 'sites/default/files/default_images/template-default-image.jpg';
				}
				
				$topimgpath = base_path () . 'sites/all/themes/waterways/assets/img/innerbox-top-img.png';
				$cruiseid = $values [$j]->id;
				
				
				//Getting Url Alias Path
				$cruisealiaspath='false';
				$urlsource = "cruisesdetailview/category/" . $cruiseid;
				$urlAliasObject = UrlAlias::getUrlAliasDetailsBySource ( $urlsource );
				
				if($urlAliasObject){
$cruisealiaspath=$urlAliasObject->alias;
				}

				
				
				
				
				$countwords = "100";
				$hoverdestext = $values [$j]->title_hover_description;
				$limitedcontent = substr($hoverdestext, 0, $countwords);
				?>
				<ul class="cruiseList">
                                
                                
				<?php 
				if($cpath!=''){
				//echo "<img src='".$cpath."'>";
				echo '<li class="dininngcrusies"><div class="cruiseboxFirst">
				<div class="cruiseboxFirstout" style="background-image:url('.$cpath.'); background-size: cover;">
					<div class="cruise-bgimage"></div>
    				<div class="cruiseboxFirstInner">'.$values [$j]->tile_title.'</div>
					</div>
    		
    		
					<div class="cruiseboxFirstover" onclick="cruiseDetails('.$cruiseid.',\''.$cruisealiaspath.'\')">
			    		<div class="cruisebox-first-inner">
			    			<img src="'.$topimgpath.'"><br>'.$limitedcontent.'<br/><a>Click to Learn More</a>
							<div class="line"></div>
						</div>
					</div>
				</div>
				</li>';
				}
				?>
				</ul>

		
			<?php
			}
			$i ++;
			$c ++;
			?>
		
		</div>
		
		<?php
		}		
		?>
		
	</div>
</div>
</div>
