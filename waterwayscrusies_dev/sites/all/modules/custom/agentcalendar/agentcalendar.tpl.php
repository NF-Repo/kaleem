<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<section id="Calendar">
    <form class="">
        <div class="container-fluid custom-container">
            <h2 class="page-heading">Calendar</h2>
            <div class="row-fluid selection-content">
                <div class="Date-contain span2">
                    <label class="label-fl" for="Category">Date</label>
                    <input type="date" form="date" class="input-block-level" >
                </div>

                <div class="Cat-contain span2">
                    <label class="label-fl" for="Category">Category</label>
                    <select class="selectpicker  input-block-level">
                        <option>All</option>
                    </select>
                </div>
                <div class="Show-contain span2">
                    <label class="label-fl" for="Category">Show</label>
                    <select class="selectpicker input-block-level">
                        <option>2 weeks</option>
                        <option>4 weeks</option>
                    </select>
                </div>
                <div class="Cruise-contain span2">
                    <label class="label-fl" for="Category">Cruise Items</label>
                    <select class="selectpicker input-block-level">
                        <option>All</option>
                    </select>
                </div>
                <div class="Searc-Contain span4">
                    <div class="Search-master">
                        <button type="submit" class="Search-btn">Search</button>
                    </div>
                </div>

            </div>

            <div class="row-fluid">

                <div id="myModal" class="modal hide fade" tabindex="-1"  align="center" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">X</button>
                        <h4 id="myModalLabel">Sep 17 2013</h4>
                    </div>
                    <div class="modal-body">
                        <h3>SENSET DINNER</h3>
                        <div class="form-inline">
                            <label class="radio">
                                <input type="radio" name="optionsRadios" id="optionsRadios1" value="option1" checked>
                                Enable
                            </label>
                            <label class="radio">
                                <input type="radio" name="optionsRadios" id="optionsRadios2" value="option2">
                                Disable
                            </label>

                            <div class="control-group">
                                <label class="control-label" for="inputEmail">Stock Remaining</label>
                                <div class="controls">
                                    <input type="text" id="inputEmail" class="input-medium" placeholder="20">
                                </div>
                            </div>

                        </div>

                    </div>

                    <div class="modal-footer" align="center">
                        <button class="btn" data-dismiss="modal" aria-hidden="true">Cancel</button>
                        <button class="btn btn-primary">Save changes</button>
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


                            <th><ul class="nav"><li>MON</li><li>15</li></ul></th>
                            <th><ul class="nav"><li>TUE</li><li>16</li></ul></th>
                            <th><ul class="nav"><li>WED</li><li>17</li></ul></th>
                            <th><ul class="nav"><li>THU</li><li>18</li></ul></th>
                            <th><ul class="nav"><li>FRI</li><li>19</li></ul></th>
                            <th><ul class="nav"><li>SAT</li><li>20</li></ul></th>
                            <th><ul class="nav"><li>SUN</li><li>21</li></ul></th>

                            <th><ul class="nav"><li>MON</li><li>22</li></ul></th>
                            <th><ul class="nav"><li>TUE</li><li>23</li></ul></th>
                            <th><ul class="nav"><li>WED</li><li>24</li></ul></th>
                            <th><ul class="nav"><li>THU</li><li>25</li></ul></th>
                            <th><ul class="nav"><li>FRI</li><li>26</li></ul></th>
                            <th><ul class="nav"><li>SAT</li><li>27</li></ul></th>
                            <th><ul class="nav"><li>SUN</li><li>28</li></ul></th>
                            <th><ul class="nav"><li>MON</li><li>29</li></ul></th>
                            <th><ul class="nav"><li>TUE</li><li>30</li></ul></th>


                            </tr>
                            </thead>

                            <tbody>
                                <tr>
                                    <td colspan="" class="port-bg">KIRKLAND PORT</td>
                                    <td colspan="30">&nbsp;</td>
                                </tr>
                                <tr>
                                    <td>SUNSET DINNER</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available"><a href="#myModal" data-toggle="modal">&nbsp;</a></td>
                                    <td class="un-available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
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
                                    <td>HAPPY HOUR</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
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
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>


                                <tr>
                                    <td colspan="" class="port-bg">SEATTLE PORT</td>
                                    <td colspan="30">&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>DINNER</td>
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
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>BRUNCH</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
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
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
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
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>

                                <tr>
                                    <td>BBQ</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
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
                                    <td class="available">&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td class="available">&nbsp;</td>
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