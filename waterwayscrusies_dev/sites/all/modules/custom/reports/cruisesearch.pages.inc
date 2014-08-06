<?php

/**
 * Displays cruise search results
 */
function cruise_search_results() {
    return drupal_render(drupal_get_form('cruise_search_results_form'));
}

function cruise_search_results_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Search') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function cruise_search_results_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/cruisesearch.css');
    global $user;

    $searchkey = '';
    if (isset($_GET['cruise_search']) && $_GET['cruise_search'] != '') {
        $searchkey = $_GET['cruise_search'];
    }

    $style = '';

    if (!in_array('staff admin', $user->roles)) {
        $style = 'style="display:none;"';
    }
    $form['#method'] = 'get';
    $form['cruise_search'] = array(
        '#id' => 'cruise_search',
        '#type' => 'textfield',
        '#title' => t(''),
        '#description' => '',
        '#default_value' => isset($searchkey) ? $searchkey : '',
        '#prefix' => '<div class="row-fluid" ' . $style . '><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
        '#attributes' => array('placeholder' => array('Search')),
    );
    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Search',
        '#prefix' => '<div class="pull-left" style="margin:14px">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('cruisesearch_export'),
        '#prefix' => '<div class="pull-left" style="margin:14px 0px 14px 0px;">',
        '#suffix' => '</div></div></div>',
    );

    $output = '';

    $header = array(
        array(
            'data' => 'Results: ' . $searchkey,
        ),
    );
    if (isset($_GET['export_btn'])) {
        cruisesearch_export($_GET);
    }
    $query = db_select("cruise", "c");
    $query->leftJoin('cruisecategory', 'g', 'c.template_category=g.cruise_category_id');
    $query->leftJoin('cruisetypes', 't', 'c.cruise_type=t.id');
    $query->condition('c.cruise_status', '1', '=');

    $query->where("c.cruise_title LIKE '%$searchkey%' or c.tile_title LIKE '%$searchkey%' "
            . "or c.title_hover_description LIKE '%$searchkey%' or g.cruise_category_name LIKE '%$searchkey%' "
            . "or t.name LIKE '%$searchkey%'");
    $query->fields('c', array(
        'id',
        'cruise_title',
        'calendar_tag',
        'tile_title',
        'title_hover_description',
    ));
    $query->fields('t', array(
        'name'
    ));
    $query->fields('g', array(
        'cruise_category_name'
    ));

//    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(5);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'cruisesdetailview/category/' . $result [$i]->id . '">READ MORE</a>';
            $cruisehtml = '';
            $cruisehtml.='<div class="container-fluid contentborder fullcontent" style="background: #fcfbf9;"><div class="row-fluid">
                            <div align="left" class="span12"><h3>' . $result [$i]->cruise_title . '</h3></div>                            
                          </div>';
            $cruisehtml.='<div class="row-fluid">
                            <div align="left" class="span12"><p>' . $result [$i]->title_hover_description . '</p></div>                            
                          </div>';
            $cruisehtml.='<div class="row-fluid">
                            <div align="left" class="span12">' . $detailsview . '</div>                            
                          </div></div>';
            $rows [] = array(
                $cruisehtml
            );
        }
    }

    $output .= '<div class="">';
    $output .= '<div style="padding-top:4%">';

    $output .= '</div>';
    $output .= theme_table(array(
                'header' => $header,
                'rows' => $rows,
                'attributes' => array(
                    'class' => array()
                ),
                'sticky' => true,
                'caption' => '',
                'colgroups' => array(),
                'empty' => t("No Records found to display"),
            )) . theme('pager');

    $output .= '</div>';
    $output .= '<div style="padding-top:4%">';

    $output .= '</div>';

    $form['outputmarkup'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
    return $form;
}

/*    Cruises Export       */

function cruisesearch_export($params) {
    $header = array(
        0 => 'Cruise Title',
        1 => 'Description'
//            0 => 'Results: ' . $searchkey
    );
    $searchkey = '';
    if (isset($params['cruise_search']) && $params['cruise_search'] != '') {
        $searchkey = $params['cruise_search'];
    }
    $query = db_select("cruise", "c");
    $query->leftJoin('cruisecategory', 'g', 'c.template_category=g.cruise_category_id');
    $query->leftJoin('cruisetypes', 't', 'c.cruise_type=t.id');
    $query->condition('c.cruise_status', '1', '=');

    $query->where("c.cruise_title LIKE '%$searchkey%' or c.tile_title LIKE '%$searchkey%' "
            . "or c.title_hover_description LIKE '%$searchkey%' or g.cruise_category_name LIKE '%$searchkey%' "
            . "or t.name LIKE '%$searchkey%'");
    $query->fields('c', array(
        'id',
        'cruise_title',
        'calendar_tag',
        'tile_title',
        'title_hover_description',
    ));
    $query->fields('t', array(
        'name'
    ));
    $query->fields('g', array(
        'cruise_category_name'
    ));

//    $query->orderBy($order, $sort);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
           
            $rows [] = array(
                $result [$i]->cruise_title,
                $result [$i]->title_hover_description
            );
        }
    }
    excel_export_data('Cruise search Reports', $header, $rows);
}
