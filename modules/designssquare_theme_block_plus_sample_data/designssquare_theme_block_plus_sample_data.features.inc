<?php
/**
 * @file
 * designssquare_theme_block_plus_sample_data.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function designssquare_theme_block_plus_sample_data_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_variable_assets_features_default_settings().
 */
function designssquare_theme_block_plus_sample_data_variable_assets_features_default_settings() {
$variable_assets = array();

$variable_assets = array(
  'ds_about__content_block__image' => array(
    'fid' => 56,
    'uid' => 1,
    'filename' => 'bio.jpg',
    'uri' => 'public://designssquare_com_theme_blocks/assets/img/bio.jpg',
    'filemime' => 'image/jpeg',
    'filesize' => 3993,
    'status' => 1,
    'timestamp' => 1402840043,
    'uuid' => '64b4a691-5b53-428a-8809-7de009d9f70f',
    'rdf_mapping' => array(),
  ),
);
return $variable_assets;
}
