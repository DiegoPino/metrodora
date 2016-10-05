<?
/**
 * @file
 * Holds block definitions for the Metrodora.
 */

/**
 * Constructs Homepage Widget Collage by facet.
 *
 * @return array
 *   Renderable array for the block.
 */
function metrodora_collagebyfacet_block() {
  $block = array();

    $query_processor = new IslandoraSolrQueryProcessor();
    $query = '*:*';
    $query_processor->buildQuery($query);
    $query_processor->internalSolrParams['type'] == "edismax";
    // Todo make this solr field configurable.
    $query_processor->solrParams['fl'] = 'PID,  mods_titleInfo_title_ms, ancestors_ms';
   $query_processor->solrParams['fq'][] = 'PID:' . "\"{$islandora_object->id}\"";
    $query_processor->solrParams['fq'][] = '-groupValue:null';
     $query_processor->solrParams['group'] = 'true';
      $query_processor->solrParams['group.field'] = 'mods_subject_topic_s';
         $query_processor->solrParams['group.sort'] = 'fgs_lastModifiedDate_dt desc';
     
    
    
    try {
      $query_processor->executeQuery(FALSE);
      $solr_results = $query_processor->islandoraSolrResult['response'];
      if ($solr_results['numFound'] == 1) {
        if (isset($solr_results['objects'][0]['solr_doc']['ancestors_ms'])) {
          $collection_pids = (array) $solr_results['objects'][0]['solr_doc']['ancestors_ms'];
        }
      }
      dpm($solr_results);
     // foreach($pids as $key => $pid) {
        //$block['key'] =
        
     // }
    
    
    
    }
    catch (Exception $e) {
      watchdog(t('Metrodora Collage by Facet Block Error', nl2br(check_plain($e->getMessage()))), NULL, WATCHDOG_WARNING);
    }
  
  

  
  
  
  
  /*
  module_load_include('inc', 'islandora_solr_search', 'includes/utilities');
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery("*:*");
  $qp->solrParams['fl'] = 'PID,PID_fullurl,fgs_label_s';
  $filters=array();
  
  //Build the first !join
  $direct_rels=array_shift($rels_ext_fields);
  
  $luceneReservedCharacters = preg_quote('+-&|!(){}[]^"~*?:\\');
    //Not nice: we are using a nested query so we need to double escape our only value, the famouse PID
  $pid= preg_replace_callback(
        '/([' . $luceneReservedCharacters . '])/',
        function($matches) {
            return '\\' . $matches[0];
        },
        $pid);
  $filters[] = format_string('_query_:{!join from=!direct_rels to=PID_uri}PID:!pid', array(
      '!direct_rels' => $direct_rels,
      '!pid' => islandora_solr_lesser_escape($pid),
    ));
  
  //*:* AND _query_:"{!join to=PID_uri from=RELS_EXT_identifies_uri_ms}PID_uri:* AND _query_:{!join from=RELS_EXT_taxonOfId_uri_ms to=PID_uri}PID:umce\\:a79c6f05\-87ce\-49e3\-b01a\-b4ac6e15ecba"*:*
  foreach($rels_ext_fields as $rels_ext_field) {
    
    $filters[] = format_string('_query_:"{!join to=PID_uri from=!rels_ext_field}PID_uri:*', array(
        '!rels_ext_field' => $rels_ext_field,
      ));
  }
  $filters[]='*:*';
 
 
  $filters_string=implode(' AND ',array_reverse($filters)).'"*:*';
  $qp->solrParams['fq'] = $filters_string;
  $xacml_restrictions=islandora_red_biodiversidad_getxacml_solrquery_restrictions();
  if (!empty($xacml_restrictions)) {
  $qp->solrParams['fq'] = $qp->solrParams['fq']." AND ( ".$xacml_restrictions." )";
  }
  
  
  $qp->solrLimit = 10000;
  $qp->executeQuery(FALSE);
 
  $results=$qp->islandoraSolrResult;
  if (isset($results['response']) && ($results['response']['numFound']>0)) {
    return array('objects'=>$results['response']['objects'],'numFound'=>$results['response']['numFound']);
    
  }
  return array();
  
  */
  
  
  /*$object = menu_get_object('islandora_object', 2);
  if ($object) {
    // If only compound cmodel objects can be compound, this isn't necessary.
    $children = islandora_compound_object_get_parts($object);
    $pid = (!empty($children)) ? $children[0] : $object->id;
    $compound_object = islandora_object_load($pid);
    $compound_info = islandora_compound_object_retrieve_compound_info($compound_object);
    if ($compound_info) {
      $module_path = drupal_get_path('module', 'islandora_compound_object');
      $jail_path = libraries_get_path('JAIL');
      $block['#attached']['js'] = array(
        "$jail_path/lib/jquery.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$jail_path/dist/jail.min.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$module_path/js/compound_jail.js" => array(
          'group' => JS_LIBRARY,
        ),
      );
      $block['#attached']['css'] = array(
        "$module_path/css/islandora_compound_object.jail_block.css",
      );
      $block['part_title'] = array(
        '#type' => 'item',
        '#markup' => t('Part of: @label (@count @obj)', array(
          '@label' => $compound_info['parent_label'],
          '@count' => $compound_info['child_count'],
          '@obj' => format_plural($compound_info['child_count'], 'object', 'objects'),
        )),
      );
      if ($compound_info['parent_url']) {
        $block['manage_link'] = array(
          '#type' => 'item',
          '#markup' => l(t('manage parent'), $compound_info['parent_url']),
        );
      }
      foreach ($compound_info['siblings'] as $sibling) {
        $sibling_object = islandora_object_load($sibling);
        if (isset($sibling_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $sibling_object['TN'])) {
          $path = 'islandora/object/' . $sibling . '/datastream/TN/view';
        }
        else {
          // Object either does not have a thumbnail or it's restricted show a
          // default in its place.
          $image_path = drupal_get_path('module', 'islandora');
          $path = "$image_path/images/folder.png";
        }
        $class = array(
          'islandora-compound-object-jail',
        );
        if ($sibling == $pid) {
          $class[] = 'islandora-compound-object-jail-active';
        }
        $img = array(
          'image' => array(
            '#theme' => 'image',
            '#path' => "$module_path/images/loader.png",
            '#attributes' => array(
              'class' => $class,
              'data-src' => url($path),
            ),
            '#href' => "islandora/object/$sibling",
          ),
          'noscript' => array(
            '#theme' => 'image',
            '#path' => $path,
            '#prefix' => '<noscript>',
            '#suffix' => '</noscript>',
          ),
        );
        $block[$sibling] = array(
          '#type' => 'container',
          '#attributes' => array(),
          'link' => array(
            '#type' => 'link',
            '#title' => t("@title", array("@title" => $sibling_object->label)),
            '#href' => "islandora/object/$sibling",
          ),
          'content' => array(
            '#theme' => 'link',
            '#text' => drupal_render($img),
            '#path' => "islandora/object/$sibling",
            '#options' => array(
              'attributes' => array(),
              'html' => TRUE,
            ),
          ),
        );
      }
    }
  }*/
  return $block;
}