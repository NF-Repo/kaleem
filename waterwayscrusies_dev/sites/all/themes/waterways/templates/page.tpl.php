<?php
/**
 * @file
 * Custom theme implementation to display a single Drupal page.
 */
// 
$usStatesObject = uc_zone_select();
$usStatesList = $usStatesObject['#options'];
//var_dump(getimagecontent());
$defaultimage = $GLOBALS['base_url'] . "/sites/all/modules/custom/homepage/homepagesliders/img/defaultslider.jpg";
?>
<script>
    function getCaptcha() {
        var chars = "0Aa1Bb2Cc3Dd4Ee5Ff6Gg7Hh8Ii9Jj0Kk1Ll2Mm3Nn4Oo5Pp6Qq7Rr8Ss9Tt0Uu1Vv2Ww3Xx4Yy5Zz";
        var string_length = 5;
        var captchastring = '';
        for (var i = 0; i < string_length; i++) {
            var rnum = Math.floor(Math.random() * chars.length);
            captchastring += chars.substring(rnum, rnum + 1);
        }
        document.getElementById("randomfield").innerHTML = captchastring;
        document.getElementById("captchavalue").value = captchastring;

    }
</script>

<?php if ($is_front) { ?>
    <style>
        @media (max-width: 480px) {
            #content{
                display: none;
            }
        }
        @media screen and (max-width: 767px) {
            #content{
                display: none;
            }
        }
    </style>
    <?php
}
?>

<div id='loading'>
    <div id='progress-bar'></div>
    <!--<div id='loader'></div>-->
</div>

<div id="transparentLoader" style="display:none;">
    <div class="transparentLoaderHolder"></div>
</div>

<!--<div class="mainWrapper">-->
<!-- header start here -->
<div id="header">
    <div class="container">
<?php print render($page['header']) ?>
    </div>
</div>
<!-- header end here -->


<!-- slider start here -->
<?php if ($is_front) { ?>
    <div id="myCarousel" class="carousel slide" style="margin-bottom: 0px;">
        <div id="slider">
         <div class="slider">
    <?php
    $response = getimagecontent();
    $items = $response["items"];
    $indicators = $response["indicators"];
    $gethomestaticdata=unserialize(homepagecontent::gethomepagecontent());
    ?>
             
             
                <ul class="customIndicators">
                <?php print $indicators; ?>

                </ul>
                 
                <div class="carousel-inner" id="slidersitems">
                    
                    <?php 
                    if(empty($indicators)){ ?>
                       <img src="<?php echo $defaultimage?>"> 
                    <?php  }else{
                    print $items;
                    }
                    
                    
                    ?>
                </div>
               

                <!-- Carousel nav -->
                <a class="carousel-control left" href="javascript:void(0);" id="caroselprev"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/carousol-leftarrow.png" /></a>
                <a class="carousel-control right" href="javascript:void(0);" id="caroselnext"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/carousol-rightarrow.png" /></a>

    <!--                <a class="carousel-control left" href="#myCarousel" data-slide="prev"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/carousol-leftarrow.png" /></a>
    <a class="carousel-control right" href="#myCarousel" data-slide="next"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/carousol-rightarrow.png" /></a>
                -->
            </div>

            <!--</div>-->
        </div>
    </div>
<?php } ?>
<!-- slider end here -->

<!-- menu start hee -->
<div id="menu">
    <div class="container">
        <div class="row-fluid">
            <div class="span12">
                <div class="navbar">
                    <div class="navbar-inner">

                        <!-- menu inner start here -->
                        <div class="menu">    
                            <!-- Visible Menu in only desktop Starts from here -->
                            <ul class="responsiveWeb nav" role="navigation">
                                <li class="firstLink dropdown">
                                    <a href="<?php echo base_path() ?>diningcruises" id="drop1" role="button"  data-toggle="dropdown" class="dropdown-toggle">DINING CRUISES<b class="caret"></b></a>

                                    <ul class="dropdown-menu" aria-labelledby="drop1" role="menu">
<?php
$data = CruiseCategory::getCruiseListByGroup();
$i = 1;
foreach ($data as $key => $values) {
    $title = CruiseCategory::getCategoryName($key);
    ?>
                                            <li><a href="<?php echo base_path() ?>diningcruises#referrel-<?php echo $i; ?>"><?php echo $title; ?></a></li>  
                                            <?php
                                            $i++;
                                        }
                                        ?>    
                                    </ul>
                                </li>
                                <li class="wwccalender"><a href="<?php echo base_path() ?>wwccalendar">CRUISE CALENDAR</a></li>
                                <li class="privateevents"><a href="<?php echo base_path() ?>private_events">PRIVATE EVENTS</a></li>
                                <li class="foodandbar"><a href="<?php echo base_path() ?>foodandbar">FOOD AND BAR</a></li>
                                
                                <?php
                                //hard coded fleet id here
                                $termid=90;
                                $fleetpath="fleet/".$termid;
                                $privateeventpath='false';
                                $urlsource = "fleet/" . $termid;
                                $urlAliasObject = UrlAlias::getUrlAliasDetailsBySource ( $urlsource );
                                if($urlAliasObject){
                                	$fleetpath=$urlAliasObject->alias;
                                }
                                
                                
                                ?>
                                
                                <li class="fleetmenu"><a href="<?php echo base_path().$fleetpath ?>">FLEET</a></li>  
                                <li class="departureports"><a href="<?php echo base_path() ?>departureports">PORTS AND DIRECTIONS</a></li>
                                <li class="connect"><a href="<?php echo base_path() ?>connectoverview" id="datasetin">CONNECT</a></li>
                            </ul>
                            <!--                             <div class="responsiveWeb"> -->
<?php //print render($page['webmenu']);  ?>
                            <!--                             </div> -->
                            <!-- Visible Menu in only desktop ends here -->

                            <!-- Visible Menu in only Mobile device Starts from here -->
                            <?php if ($is_front) { ?>
                                <!--                                 <div class="responsiveMobile" id="mobilerequestProposal"> -->
    <?php //print render($page['mobilemenu']);  ?>
                                <!--                                 </div> -->
                                <ul class="responsiveMobile">
                                    <li><a href="<?php echo base_path() ?>wwccalendar">BOOK NOW</a></li>
                                    <li><a href="#myModalrequest" role="button" data-toggle="modal">REQUEST A  PROPOSAL</a></li>
                                    <li class="wwccalender"><a href="<?php echo base_path() ?>wwccalendar">CRUISE CALENDAR</a></li>
                                    <!-- <li class="firstLink"><a href="<?php echo base_path() ?>diningcruises">DINING CRUISES</a></li> -->
                                    <li class="firstLink dropdown">
                                        <a href="<?php echo base_path() ?>diningcruises" id="drop1" role="button"  data-toggle="dropdown" class="dropdown-toggle">DINING CRUISES<b class="caret"></b></a>
                                        <ul class="dropdown-menu" aria-labelledby="drop1" role="menu">
    <?php
    $mobiledata = CruiseCategory::getCruiseListByGroup();
    $m = 1;
    foreach ($mobiledata as $key => $values) {
        $mobiletitle = CruiseCategory::getCategoryName($key);
        ?>
                                                <li><a href="<?php echo base_path() ?>diningcruises#referrel-<?php echo $m; ?>"><?php echo $mobiletitle; ?></a></li>  
                                                <?php
                                                $m++;
                                            }
                                            ?>
                                        </ul>
                                    </li>


                                    <li class="privateevents"><a href="<?php echo base_path() ?>private_events">PRIVATE EVENTS</a></li>
                                    <li class="departureports"><a href="<?php echo base_path() ?>departureports">PORTS AND DIRECTIONS</a></li>
                                    <li class="fleetmenu"><a href="<?php echo base_path() ?>fleet/90">FLEET</a></li>
                                    <li class="foodandbar"><a href="<?php echo base_path() ?>foodandbar">FOOD AND BAR</a></li>
                                    <li class="connect"><a href="<?php echo base_path() ?>connectoverview">CONNECT</a></li>
                                    <li><a href="<?php echo base_path() ?>contactus/add">CONTACT</a></li>
                                </ul>

<?php } else { ?>

                                <div class="responsiveMobile innerpagesMenuStyle" id="mobilerequestProposal">
                                    <div class="mobilemenuicon" align="center">
                                        <div class="mobileIconWrapper">
                                            <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
                                                <span class="icon-bar"></span>
                                                <span class="icon-bar"></span>
                                                <span class="icon-bar"></span>
                                                <span class="mobilemenuTitle">MENU</span>
                                            </a>
                                        </div>
                                    </div>
                                    <div class="nav-collapse collapse">
                                        <ul class="nav">
                                            <li><a href="<?php echo base_path() ?>wwccalendar">BOOK NOW</a></li>
                                            <li><a href="#myModalrequest" role="button" data-toggle="modal">REQUEST A  PROPOSAL</a></li>
                                            <li class="wwccalender"><a href="<?php echo base_path() ?>wwccalendar">CRUISE CALENDAR</a></li>
                                            <li class="firstLink dropdown">
                                                <a href="<?php echo base_path() ?>diningcruises"  data-toggle="dropdown" class="dropdown-toggle">DINING CRUISES<b class="caret"></b></a>
                                                <ul class="dropdown-menu">
    <?php
    $mobiledata = CruiseCategory::getCruiseListByGroup();
    $m = 1;
    foreach ($mobiledata as $key => $values) {
        $mobiletitle = CruiseCategory::getCategoryName($key);
        ?>
                                                        <li><a href="<?php echo base_path() ?>diningcruises#referrel-<?php echo $m; ?>"><?php echo $mobiletitle; ?></a></li>  
                                                        <?php
                                                        $m++;
                                                    }
                                                    ?>
                                                </ul>
                                            </li>
                                            <li class="privateevents"><a href="<?php echo base_path() ?>private_events">PRIVATE EVENTS</a></li>
                                            <li class="departureports"><a href="<?php echo base_path() ?>departureports">PORTS AND DIRECTIONS</a></li>
                                            <li class="fleetmenu"><a href="<?php echo base_path() ?>fleet/90">FLEET</a></li> 
                                            <li class="foodandbar"><a href="<?php echo base_path() ?>foodandbar">FOOD AND BAR</a></li>
                                            <li class="connect"><a href="<?php echo base_path() ?>connectoverview">CONNECT</a></li>
                                            <li><a href="<?php echo base_path() ?>contactus/add">CONTACT</a></li>
                                        </ul>
                                    </div>
                                </div>

<?php } ?>
                            <!-- Visible Menu in only Mobile device ends here -->
<?php //print $navbar_menu  ?>
                        </div>
                    </div>
                </div>
                <!-- menu inner end here -->
            </div>
        </div><!-- row-fluid end -->
    </div>
</div>
<!-- menu end hee -->




<!-- content start here -->
<div id="content">

<?php print $messages; ?>

    
  
    <!-- sidebar block starts from here  -->
<?php
if ($page['sidebar_first']) {
    echo '<div class="container">';
    echo '<div class="connectMain">';
    echo '<div class="row-fluid blogMain">';
    echo '<div class="span4">';
    print render($page['sidebar_first']);
    echo '</div>';
}
?>
    <!-- sidebar block ends from here  -->


    <?php
    if ($page['sidebar_first']) {
        echo '<div class="span8">';
        echo '<div class="blogInner">';
    }
    ?>
    
    
    <?php 
    print render($page['content']) ?>
    <?php
    if ($page['sidebar_first']) {
        echo '</div>';
        echo '</div>';
        echo '</div>';
        echo '</div>';
        echo '</div>';
    }
    ?>

</div>
<!-- content end here -->
<!-- footer start here -->
<div id="footer">
    <div class="container">
        <div class="footerInner">
            <div class="row-fluid">
<?php print render($page['footer']) ?>
            </div>
            <!-- row-fluid end -->
        </div>
    </div>
</div>
<!-- footer end here -->

<!--</div>-->

<!--pop starts-->

<!-- REQUEST A PROPOSAL modal popup start here -->
<div id="myModalrequest" class="modal hide" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-header">
        <!--<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>-->
        <button type="button" id="rquestquoteClose"  class="close">x</button>
        <h3 id="myModalLabel">REQUEST A PROPOSAL</h3>
    </div>

    <form name="requestQuoteForm" id="requestQuoteForm">
        <div id="requestQuoteformcontent">
            <div class="modal-body" >
                <div class="row-fluid">
                    <div class="span12">
                        <div>Start planning your event by requesting a quote information!  Call us at 206-223-2060 or fill out this form and one of our event planners will contact you within 48 hours.</div>
                        <div class="alert-info"> *This information will be used only to contact you regarding your request,  and it will not be sold or transferred to anyone else.</div>
                        <div class="row-fluid">
                            <div class="span12">
                                <fieldset>
                                    <legend style="margin-bottom: 10px;">Type of Event</legend>
                                    <div class="row-fluid">
                                        <div class="span6">
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="Wedding (Ceremony/Reception)"> Wedding (Ceremony/Reception)
                                            </label>
                                            <label class="checkbox" name="quoteEventtype[]">
                                                <input type="checkbox" name="quoteEventtype[]" value="Reharsal Dinner"> Reharsal Dinner
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="Corporate Event"> Corporate Event
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="Birthday"> Birthday
                                            </label>
                                        </div>
                                        <div class="span6">
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="Anniversary"> Anniversary
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="School Event (High School)"> School Event (High School)
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="School Event (College)"> School Event (College)
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventtype[]" value="EventTypeOther" id="OtherText"> Other
                                            </label>
                                            <label class="input">
                                                <textarea placeholder="Enter Text.." rows="4" name="quoteEventtypeotherText" id="quoteEventtypeotherText"></textarea>
                                            </label>
                                        </div>
                                    </div><!-- row-fluid end -->

                                    <div class="row-fluid">
                                        <div class="span12">
                                            <label class="radiobox">
                                                <input type="radio" name="myEventSelection[]" value="privateYacht" id="privateYacht"> I prefer to charter a private yacht for my event.
                                            </label>
                                            <label class="radiobox">
                                                <input type="radio" name="myEventSelection[]" value="hostEventAboard" id="hostEventAboard"> I can host my event aboard a scheduled dining cruise.
                                            </label>
                                        </div>
                                    </div><!-- row-fluid end -->

                                    <hr/>

                                    <div class="row-fluid">
                                        <div class="span12">
                                            <label class="input-medium">
                                                <input type="text" placeholder="Company/Organization" class="inputfieldSmall inputfiledStyle" name="quoteCompany" id="quoteCompany"/>
                                            </label>
                                        </div>
                                    </div>

                                    <div class="row-fluid">
                                        <div class="span6">
                                            <label class="input-medium">
                                                <input type="text" placeholder="*First Name" class="inputfieldSmall inputfiledStyle" name="quoteFirstName" id="quoteFirstName" required />
                                            </label>
                                        </div>
                                        <div class="span6">
                                            <label class="input-medium">
                                                <input type="text" placeholder="*Last Name" class="inputfieldSmall inputfiledStyle" name="quoteLastName" id="quoteLastName" required />
                                            </label>
                                        </div>
                                    </div>

                                    <div class="row-fluid">
                                        <div class="span6">
                                            <label class="input-medium">
                                                <input type="text" placeholder="*Email" class="inputfieldSmall inputfiledStyle" name="quoteEmail" id="quoteEmail"/>
                                            </label>
                                        </div>
                                        <div class="span6">
                                            <label class="input-medium">
                                                <input type="text" placeholder="*Confirm Email" class="inputfieldSmall inputfiledStyle" name="quoteconfirmEmail" id="quoteconfirmEmail"/>
                                            </label>
                                        </div>
                                    </div>

                                    <div class="row-fluid bottommarginmedium" >
                                        <div class="span12">
                                            <label>I prefer to be contacted via</label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="preferdcontact[]" value="preferdphone"> Phone
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="preferdcontact[]" value="preferedemail"> Email
                                            </label>
                                        </div>
                                    </div>

                                    <div class="row-fluid">
                                        <div class="span12">
                                            <label class="input-medium" style="font-size:12px;">
                                                <input type="text" placeholder="Phone Number" class="inputfieldSmall inputfiledStyle" name="quotePhoneNumber" id="quotePhoneNumber"/>
<!--                                                <span style="font-size:12px;opacity:0.5;">(555) 555-5555 </span></label>-->
                                        </div>
                                    </div>

                                    <div class="row-fluid">
                                        <div class="span6">

                                            <label>Best time to call?</label>
                                            <select name="quoteBestTimecall" class="selectFieldStyle selectFieldSmall" id="quoteBestTimecall">
                                                <option value="">Please Select One</option>
                                                <option value="Early Morning 8-10am">Early Morning 8-10am</option>
                                                <option value="Mid Morning 10am-1pm">Mid Morning 10am-1pm</option>
                                                <option value="Afternoon 1-5pm">Afternoon 1-5pm</option>
                                                <option value="Anytime 8am-8pm">Anytime 8am-5pm</option>
                                            </select>
                                        </div>
                                        <div class="span6">

                                        </div>
                                    </div><!-- row-fluid end -->
                                    <hr/>
                                    <div class="row-fluid">
                                        <div class="span6">
                                            <div class="row-fluid">
                                                <div class="span12">
                                                    <div  id="quoteEventDate" class="input-append date">
                                                        <label>Your preferred date of the event?</label>
                                                        <input data-format="dd/MM/yyyy" class="dateFieldStyle dateFieldMedium" type="text" id="quoteEventDatetext" readonly="true"/>
        <!--                                                 <span class="add-on"> -->
        <!--                                                     <i class="icon-calendar"></i> -->
                                                        <!--                                                 </span> -->
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row-fluid">
                                                <div class="span12">
                                                    <div data-format="dd/MM/yyyy" type="text" id="quoteEventDatediv"></div>  
                                                    <!--                                            <link rel="stylesheet" href="/resources/demos/style.css">
                                                                                                <link rel="stylesheet" href="//code.jquery.com/ui/1.10.4/themes/smoothness/jquery-ui.css">                                              -->
                                                </div>

                                            </div>

                                        </div>
                                        <div class="span6">
                                                    <label>Estimated number of guests<span title="This field is required." style="color:#C60F13;">*</span></label>
                                                    <input type="text" class="inputfieldSmall inputfiledStyle" name="quoteGuestsNo" id="quoteGuestsNo" required />
                                                </div>
                                    </div><!-- row-fluid end -->
                                    <div class="row-fluid">
                                        <div class="span6">
                                            <div data-format="dd/MM/yyyy" type="text" id="quoteEventDatediv"></div>  
<!--                                            <link rel="stylesheet" href="/resources/demos/style.css">
                                            <link rel="stylesheet" href="//code.jquery.com/ui/1.10.4/themes/smoothness/jquery-ui.css">                                              -->
                                        </div>
                                    </div>
                                    
                                    <div class="row-fluid">
                                        <div class="span6">
                                            <label>Your preferred time of the event</label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventTime[]" value="Morning"> Morning
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventTime[]" value="Afternoon"> Afternoon
                                            </label>
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteEventTime[]" value="Evening"> Evening
                                            </label>
                                        </div>
<!--                                        <div class="span6">
                                            <label>Your preferred venue?</label>
                                            <select name="quotepreferredvenue" class="selectFieldStyle selectFieldSmall" id="quotepreferredvenue">
                                                <option value="">Please Select One</option>
                                                <option value="Emerald Star (220 ppl)">Emerald Star (220 ppl)</option>
                                                <option value="Olympic Star (125 ppl)">Olympic Star (125 ppl)</option>
                                                <option value="The Destiny (40 ppl)">The Destiny (40 ppl)</option>
                                            </select>
                                        </div>-->
                                    </div>
                                    <div class="row-fluid">
                                        <div class="span12">
                                            <textarea rows="4" placeholder="Estimated Budget" class="textareaStyle topmarginmedium" name="estimatedBudget" id="estimatedBudget"></textarea>
                                        </div>
                                    </div>
                                    <!-- row-fluid end -->                                    

                                    <hr/>
                                    <div class="row-fluid">
                                        <div class="span12">
                                            <label>How did you find out about us?</label>
                                            <select name="quotefindby" class="selectFieldStyle selectFieldSmall" id="quotefindby">
                                                <!--                                            <option value="Website">Website</option>-->
                                                <option value="">Please Select One</option>
                                                <option value="Internet Search">Internet Search</option>
                                                <option value="Online Ads">Online Ads</option>
                                                <option value="Daily Deal">Daily Deal</option>
                                                <option value="Seattle Times">Seattle Times</option>
                                                <option value="Pandora">Pandora</option>
                                                <option value="Magazine">Magazine</option>
                                                <option value="Newspaper">Newspaper</option>
                                                <option value="Radio or TV">Radio or TV</option>
                                                <option value="Email">Email</option>
                                                <option value="Gift Certificate">Gift Certificate</option>
                                                <option value="Concierge">Concierge</option>
                                                <option value="Word of Mouth">Word of Mouth</option>
                                                <option value="Return Customer">Return Customer</option>

                                            </select>

                                        </div>
                                    </div><!-- row-fluid end -->

                                    <div class="row-fluid topmarginmedium">
                                        <div class="span12">
                                            <label class="checkbox">
                                                <input type="checkbox" name="quoteSpecialEmailenabled[]" value="Yes"> 
                                                Yes, I would like to receive Waterways specials by email.
                                            </label>
                                            <!-- 	                                            <label>Yes, I would like to receive Waterways specials by email.</label> -->
                                        </div>
                                    </div><!-- row-fluid end -->

                                    <hr/>
                                    <div class="row-fluid">
                                        <div class="span12">
                                            <textarea placeholder="Comments" class="textareaStyle" rows="4" class="span10" id="quoteUserComments"></textarea>
                                        </div>
                                    </div><!-- row-fluid end -->
                                </fieldset>



                            </div>

                        </div><!-- row-fluid end -->

                    </div>
                </div><!-- row-fluid end -->
            </div>
            <div class="modal-footer">
                <!--<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>-->
                <!--<button class="btn btn-primary" id="requestQuotemodalSubmit">Submit Request</button>-->
                <span class="loadergif" style="display:none;"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/status-active.gif" /></span>                
                <!--<input type="button" class="btn btn-primary" id="requestQuotemodalbtn" value="Submit Request"/>-->
                <div class="requestbutton">
                    <input type="button" class="requestbuttonInner" id="requestQuotemodalbtn" value="Submit Request"/>
                </div>
            </div>
        </div>
    </form>





</div>

<!-- REQUEST A PROPOSAL modal popup end here -->
<div id="myModalthankyou" class="modal hide" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-header">
        <button type="button" id="rquestquotethanksClose"  class="close">x</button>
        <h3 id="myModalLabel">REQUEST A PROPOSAL</h3>
    </div>
    <div class="modal-body">

        <div id="requestQuotethanksContent">

            <h2 class="text-info">Thankyou for the request!</h2>

            <p>Helvetica aute dolor, ugh marfa et pug hashtag cillum four loko church-key brunch terry richardson. Sint pork belly fanny pack, fingerstache proident food truck dolore master cleanse labore. Put a bird on it high life irony godard, sint fanny pack bicycle rights typewriter banksy placeat.</p>

        </div>

    </div>
</div>


<!--Feedback Thanks Page-->

<div id="myModalfeedbackthankyou" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">


    <div class="modal-header">
        <button type="button" id="feedbackthanksClose"  class="close">x</button>
        <h3 id="myModalLabel">FEEDBACK FORM</h3>
    </div>
    <div class="modal-body">

        <div id="feedbackthanksContent">

            <h2 class="text-sucess">Thankyou for  your valuable feedback !</h2>


        </div>

    </div>


</div>
<!--End of Feedback Thanks Page-->








<!-- FEEDBACK FORM modal popup start here -->
<div id="myModalfeed" class="modal hide fade" 
     tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">

    <div class="modal-header">
        <!--<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>-->
        <button type="button" id="feedbackmodalClose"  class="close">x</button>


        <h3 id="myModalLabel">FEEDBACK FORM</h3>
    </div>
    <form id="feedbackform" name="feedbackform">
        <div class="modal-body">
            <div class="row-fluid">
                <div class="span12">
                    We would love to hear your thoughts, concerns or problems with anything so we can improve!<br>
                    <small> Required fields*</small>


                    <div class="row-fluid">
                        <div class="span12">

                            <fieldset>

                                <div class="row-fluid">
                                    <div class="span12">
                                        <br>
                                        <label>Feedback Type</label>
                                        <div class="row-fluid">
                                            <div class="span3"> <label class="radio">
                                                    <input type="radio" name="feedbacktype" value="Comments" checked> Comments
                                                </label></div>
                                            <div class="span3">
                                                <label class="radio">
                                                    <input type="radio" name="feedbacktype" value="Bug Reports"> Bug Reports
                                                </label>

                                            </div>
                                            <div class="span3">
                                                <label class="radio">
                                                    <input type="radio" name="feedbacktype" value="Questions"> Questions
                                                </label>

                                            </div>
                                        </div><!-- row-fluid end -->


                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span12">
                                        <!--                                         <label ></label> -->
                                        <textarea placeholder="*Describe Feedback" rows="4" class="textareaStyle" id="feedbackDescription" name="feedbackDescription"></textarea>

                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <!--                                         <label></label> -->
                                        <input type="text" placeholder="*First Name" class="inputfieldSmall inputfiledStyle" name="feedbackFirstName" id="feedbackFirstName"/>

                                    </div>
                                    <div class="span6">
                                        <!--                                         <label></label> -->
                                        <input type="text" placeholder="*Last Name" class="inputfieldSmall inputfiledStyle" name="feedbackLastName" id="feedbackLastName"/>

                                    </div>
                                </div><!-- row-fluid end -->
                                <!--                                 <hr/> -->
                                <div class="row-fluid">
                                    <div class="span6">
                                        <!--                                         <label>*</label> -->
                                        <input type="text" placeholder="*Email" class="inputfieldSmall inputfiledStyle" name="feedbackEmail" id="feedbackEmail"/>

                                    </div>
                                    <div class="span6">
                                        <!--                                         <label>*</label> -->
                                        <input type="text" placeholder="*Confirm Email" class="inputfieldSmall inputfiledStyle" name="feedbackConfirmemail" id="feedbackConfirmemail"/>

                                    </div>
                                </div><!-- row-fluid end -->

                            </fieldset>

                        </div>

                    </div><!-- row-fluid end -->

                </div>
            </div><!-- row-fluid end -->
        </div>

        <div class="modal-footer">
            <!--<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>-->
            <!--                <button class="btn btn-primary" id="feedback">Submit</button>-->
            <!--feedbackformSubmit-->
            <span class="loadergif" style="display:none;"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/status-active.gif" /></span> 
            <!--<input type="button" class="btn btn-primary" id="feedbackformSubmit" value="Submit"/>-->
            <div class="requestbutton">
                <input type="button" class="requestbuttonInner" id="feedbackformSubmit" value="Submit"/>
            </div>
        </div>
    </form>
</div>

<!-- REQUEST A PROPOSAL modal popup end here -->




<!-- Corporate Giving FORM modal popup start here -->
<div id="corporateGiving" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-header">
        <!--<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>-->



        <h3 id="myModalLabel">Corporate Giving <button type="button" id="corporatemodalClose"  class="close">x</button></h3>
    </div>
    <div class="modal-body">
        <div class="row-fluid">
            <div class="span12">
                Echo park you probably have not hread of them bespoke cred,voluptate brooklyn aute forage hoodle sriracha quinoa Tonx mlkshk est umanmi<br>
                <small> Required fields*</small>
                <hr>

                <div class="row-fluid">
                    <div class="span12">
                        <form id="corporateGivingform" name="feedbackform">
                            <fieldset>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <div class="row-fluid"><div class="span12">
                                                <input type="text" placeholder="*Organization requesting donation" name="donation" id="donation" required/>
                                            </div>
                                        </div><!-- row-fluid end -->


                                    </div>
                                    <div class="span6 input-append date" id="corporateEventDate">
                                        <input data-date-format="mm/dd/yy" type="text" name="date" id="corporateEventDateText" required/>
                                        <span class="add-on">
                                            <i class="icon-calendar"></i>
                                        </span>
                                    </div>

                                </div><!-- row-fluid end -->


                                <div class="row-fluid">
                                    <div class="span12">
                                        <textarea placeholder="*Description of event and community it serves (Please note if your organization NW based)" rows="4" class="span10 corporate-desc" name="description" required></textarea>
                                    </div>

                                </div><!-- row-fluid end -->


                                <div class="row-fluid">
                                    <div class="span12">
                                        <textarea placeholder="*Goal your event is trying to reach" rows="4" class="span10" name="eventgoal" id="eventgoal" required></textarea>

                                    </div>
                                </div>
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <input type="text" placeholder="*First Name" name="firstname" id="corporateFirstName" required/>

                                    </div>
                                    <div class="span6">
                                        <input type="text" placeholder="*Last Name" name="lastname" id="corporateLastName" required/>

                                    </div>
                                </div><!-- row-fluid end -->
                                <div class="row-fluid">
                                    <div class="span6">
                                        <input type="text" placeholder="*Address" name="address" id="corporateAddress" required/>

                                    </div>

                                </div><!-- row-fluid end -->
                                <div class="row-fluid">
                                    <div class="span6">
                                        <input type="text" name="address" id="address1" />
                                    </div>

                                </div><!-- row-fluid end -->
                                <div class="row-fluid">
                                    <div class="span6">
                                        <input type="text" placeholder="*City" name="city" id="corporateCity" required/>

                                    </div>
                                    <div class="span3">
                                        <select name="corporatestate" id="corporatestate" class="span12">
                                            <option value="">*State</option>
                                       <?php
                                        foreach ($usStatesList as $option) {
                                        echo '<option>' . $option . '</option>';
                                            }
                                       ?>
                                          
                                        </select>
                                    </div>
                                    <div class="span3">
                                        <input type="text" placeholder="*Zipcode" class="span12" name="zipcode" id="corporateZipcode" required/>

                                    </div>
                                </div>

                                <div class="row-fluid">
                                    <div class="span6">
                                        <label class="input-medium">
                                            <input type="text" placeholder="*Phone Number" id="corporatePhone" name="phoneNumber" required/>
                                            <small>(555) 555-5555</small>
                                        </label>
                                    </div>
                                </div>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <input type="text" placeholder="*Email" name="corporateEmail" id="corporateEmail" required/>

                                    </div>
                                    <div class="span6">
                                        <input type="text" placeholder="*Confirm Email" name="corporateConfirmemail" id="corporateConfirmemail" required/>

                                    </div>
                                </div><!-- row-fluid end -->

                                <div class="row-fluid">
                                    <div class="span6 captcha-field">

                                        <div id="captcha">
                                            <div id="captcha_gen">
                                                <label align="center" id="randomfield"></label>
                                                <input type="hidden" id="captchavalue"/>
                                            </div>
                                        </div>
                                        <div class="requestbutton captcha-refresh">
                                            <input type="button" class="requestbuttonInner" onClick="getCaptcha();" value="Refresh">
                                        </div>


                                    </div></div>
                                <div class="row-fluid">
                                    <div class="span8">
                                        <label>*Please enter the above code into this box</label>
                                        <input type="text" name="captcha" id="captcha-code"/>
                                    </div>
                                </div>
                            </fieldset>

                        </form>

                    </div>

                </div><!-- row-fluid end -->

            </div>
        </div><!-- row-fluid end -->
    </div>

    <div class="modal-footer">
        <!--<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>-->
        <!--                <button class="btn btn-primary" id="feedback">Submit</button>-->
        <!--feedbackformSubmit-->
        <span class="loadergif" style="display:none;"><img src="<?php echo base_path() . path_to_theme(); ?>/assets/img/status-active.gif" /></span> 
        <!--<input type="button" class="btn btn-primary" id="feedbackformSubmit" value="Submit"/>-->
        <div class="requestbutton">
            <input type="button" class="requestbuttonInner" id="corporateformSubmit" value="Send"/>
        </div>
    </div>

</div>

<!-- Corporate Giving FORM modal popup ends here -->





<!--Corporate Giving Thanks Page-->

<div id="myModalcorporategivingthankyou" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">


    <div class="modal-header">
        <button type="button" id="corporategivingthanksClose"  class="close">x</button>
        <h3 id="myModalLabel">Corporate Giving</h3>
    </div>
    <div class="modal-body">

        <div id="corporategivingthanksContent">

            <h2 class="text-sucess">Thankyou for submitting!</h2>


        </div>

    </div>


</div>
<!--End of Corporate Giving Thanks Page-->