<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php

//print $output;
$term1 = taxonomy_term_load($output);
//dpm($term);
$nodedata = node_load(arg(1));
//dpm($nodedata);
$termdata = array();
if (!empty($nodedata->field_case_tags)) {
    foreach ($nodedata->field_case_tags['und'] as $term) {

        $termdata[] = $term['taxonomy_term']->tid;
    }
}
if (in_array($output, $termdata)) {
    echo l($term1->name, "cases/" . $term1->tid, array("attributes" => array("class" => "active")));
} else {
    echo l($term1->name, "cases/" . $term1->tid);
}
$nodedata = '';
$termdata = '';
?>