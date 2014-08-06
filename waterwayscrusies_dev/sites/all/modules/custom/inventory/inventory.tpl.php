<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<section id="Inventory">

    <form class="">
        <div class="container-fluid custom-container">
            <h2 class="page-heading">Inventory</h2>
            <div class="row-fluid selection-content">
                <div class="Cat-contain span3">
                    <label class="label-fl" for="Category">Date</label>
                    <input type="date" form="date" class="input-block-level" >
                </div>
                <div class="Show-contain span3">
                    <label class="label-fl" for="Category">Show</label>
                    <select class="selectpicker input-block-level">
                        <option>2 weeks</option>
                        <option>4 weeks</option>
                    </select>
                </div>
                <div class="Cruise-contain span3">
                    <label class="label-fl" for="Category">Category</label>
                    <select class="selectpicker input-block-level">
                        <option>All</option>
                    </select>
                </div>
                <div class="span3">
                    <div class="Search-master">
                        <button type="submit" class="Search-btn">Search</button>
                    </div>
                </div>

            </div>
            
            
            <div class="row-fluid">
                <div class="span12">
                    <div class="controllers">
                        <a class="left-control"  data-slide="prev" href="#"><img src="<?php echo base_path() . path_to_theme() ?>/images/left-arrow.png" /></a>
                        <h3 align="center" class="list-month">September 2013</h3>
                        <a class="right-control"  data-slide="prev" href="#"><img src="<?php echo base_path() . path_to_theme() ?>/images/right-arrow.png" /></a>
                        <div style="clear:both"></div>
                    </div>  

                </div>
            </div>


            <div class="row-fluid">
                <div class="span12">
                    <div class="table-responsive" style="text-align:center;">
                        <table id="sample-table-1" class="table table-bordered">
                            <thead valign="middle">
                                <tr valign="middle">
                                    <th><p>ITEM</p></th>
                            <th><ul class="nav"><li>MON</li><li>1</li></ul></th>
                            <th><ul class="nav"><li>TUE</li><li>2</li></ul></th>
                            <th><ul class="nav"><li>WED</li><li>3</li></ul></th>
                            <th><ul class="nav"><li>THU</li><li>4</li></ul></th>
                            <th><ul class="nav"><li>FRI</li><li>5</li></ul></th>
                            <th><ul class="nav"><li>SAT</li><li>6</li></ul></th>
                            <th><ul class="nav"><li>SUN</li><li>7</li></ul></th>


                            <th><ul class="nav"><li>MON</li><li>8</li></ul></th>
                            <th><ul class="nav"><li>TUE</li><li>9</li></ul></th>
                            <th><ul class="nav"><li>WED</li><li>10</li></ul></th>
                            <th><ul class="nav"><li>THU</li><li>11</li></ul></th>
                            <th><ul class="nav"><li>FRI</li><li>12</li></ul></th>
                            <th><ul class="nav"><li>SAT</li><li>13</li></ul></th>
                            <th><ul class="nav"><li>SUN</li><li>14</li></ul></th>




                            </tr>
                            </thead>

                            <tbody>
                                <tr>
                                    <td class="port-bg">KIRKLAND PORT</td>
                                    <td colspan="14"></td>
                                </tr>
                                <tr>
                                    <td>SUNSET DINNER</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>&nbsp;</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>50</td>
                                    <td>&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>HAPPY HOUR</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>90</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>70</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>90</td>
                                </tr>


                                <tr>
                                    <td class="port-bg">SEATTLE PORT</td>
                                    <td colspan="14"></td>
                                </tr>

                                <tr>
                                    <td>DINNER</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>40</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>BRUNCH</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>40</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>80</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>50</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>


                                <tr>
                                    <td>LUNCH</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>BBQ</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>60</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>40</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>30</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>


                            </tbody>
                        </table>
                    </div><!-- /.table-responsive -->
                </div><!-- /span -->
            </div>

        </div>
    </form>
</section>