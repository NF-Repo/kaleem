<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script>

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
         $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
    });
</script>

<div id="content" class="aboutusPagesContainer">
    <div class="contentDetail">
        <!-- sunset dining start here -->
        <div class="contentbox-inner-template">            
            <div class="detailBackLink">
                <a href="<?php echo base_path().'aboutuscareers'; ?>"><img src="<?php echo base_path().  path_to_theme() ?>/images/back_arrow.png" />&nbsp;BACK TO CAREERS</a>
            </div>
            <div class="jobContainer jobContainerMargins">
                <?php print $jobtemplatedetails; ?>
            </div>
        </div>
</div>

</div>

