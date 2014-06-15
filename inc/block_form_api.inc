<?php
define('MODULE_DEST', 'public://designssquare_com_theme_blocks/assets');
define('MAX_SIZE_LIMIT_DS', (int)(ini_get('upload_max_filesize')));

/*********General********/
function ds_block_edit_form($form, &$form_submit){

    switch($form_submit['build_info']['args'][0]){
        case 'ds_social':
            $form += _social_block_form();
            $form_title = 'Block Widget - Social';
            break;
        case 'ds_about':
            $form += _about_block_form();
            $form_title = 'Block Widget - About';
            break;
        case 'ds_copy_rights':
            $form += _copy_rights_block_form();
            $form_title = 'Block Widget - Copy Rights';
            break;
        case 'ds_announcement':
            $form += _announcement_block_form();
            $form_title = 'Block Widget - Announcement';
            break;
        case 'ds_recent_tweets':
            $form += _recent_tweets_block_form();
            $form_title = 'Block Widget - Recent Tweets';
            break;
    }

   $form['form_header'] = array(
        '#markup' => '<h2>'.$form_title.'</h2>',
        '#weight' => -15,
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
    );
    return $form;
}

function ds_block_edit_form_submit($form, &$form_state) {

    switch($form_state['build_info']['args'][0]){
        case 'ds_copy_rights':
            _copy_rights_block_form_process($form_state);
            break;
        case 'ds_announcement':
            _announcement_block_form_process($form_state);
            break;
        case 'ds_about_sidebar':
        case 'ds_about':
            _about_block_form_process($form_state);
            break;
        case 'ds_social':
            _social_block_form_process($form_state);
            break;
        case 'ds_recent_tweets':
            _recent_tweets_block_form_process($form_state);
            break;
    }
// Display a message upon successful submission.
    drupal_set_message(t("Widget was updated successfully!"));
}

/******BLOCK - ABOUT ********/
function _about_block_form()
{
    return array(
        'ds_about__content_block__title' => array(
            '#type' => 'textfield',
            '#prefix' => t('Title'),
            '#default_value' => variable_get('ds_about__content_block__title', ''),
        ),
        'ds_about__content_block__content' => array(
            '#type' => 'textarea',
            '#rows' => '5',
            '#prefix' => t('Content'),
            '#default_value' => variable_get('ds_about__content_block__content', ''),
        ),
        'ds_about__content_block__reference' => array(
            '#type' => 'textfield',
            '#prefix' => t('Page To Reference'),
            '#suffix' => t('(i.e. about/about-company/designs-square-com)'),
            '#default_value' => variable_get('ds_about__content_block__reference', ''),
        ),
        'ds_about__content_block__image' => array(
            '#title' => t('Image'),
            '#type' => 'managed_file',
            '#description' => t('Upload a file, allowed extensions: jpg, jpeg, png, gif'),
            '#default_value' => variable_get('ds_about__content_block__image', ''),
            '#upload_location' => MODULE_DEST.'/img',
            '#upload_validators' => array(
                'file_validate_extensions' => array('jpg jpeg png gif'),
                // Pass the maximum file size in bytes
                'file_validate_size' => array(MAX_SIZE_LIMIT_DS*1024*1024),
            ),
        )
    );
}

function _about_block_form_process($form_state){
    variable_set('ds_about__content_block__title', $form_state['values']['ds_about__content_block__title']);
    variable_set('ds_about__content_block__content', $form_state['values']['ds_about__content_block__content']);

    //make file permanent
    if(is_numeric($form_state['values']['ds_about__content_block__image']) && $form_state['values']['ds_about__content_block__image'] > 0){
        $file = file_load($form_state['values']['ds_about__content_block__image']);

        if($file->status !== FILE_STATUS_PERMANENT){
            // Change status to permanent.
            $file->status = FILE_STATUS_PERMANENT;
            //all permanent files need an entry in the 'file_usage' table
            file_usage_add($file, 'designssquare_com_theme_blocks', 'ds_about__content_block__image', '1');
            // Save.
            file_save($file);
        }

    }
    //before setting  new file, lets ensure we remove previous from the file_managed, file_usage table
    if($previous_fid = variable_get('ds_about__content_block__image', FALSE)){
        if($file = file_load($previous_fid)){
            file_delete($file);
            file_usage_delete($file,'designssquare_com_theme_blocks');
            file_unmanaged_delete($file->uri);
        }
    }
    variable_set('ds_about__content_block__image', $form_state['values']['ds_about__content_block__image']);
    variable_set('ds_about__content_block__reference', $form_state['values']['ds_about__content_block__reference']);
}

function _about_block_feed()
{

    $title = variable_get('ds_about__content_block__title', '');
    $content = variable_get('ds_about__content_block__content', '');
    $img_fid = variable_get('ds_about__content_block__image', '');
    $reference_page = '/'.variable_get('ds_about__content_block__reference', '#');
    if(!empty($img_fid)){
       $file = file_load($img_fid);
        $img_url = (isset($file) && isset($file->uri)) ? file_create_url($file->uri) : '';
    }else{
        $img_url = '#';
    }

    module_load_include('module', 'contextual');
    $contextual_links = contextual_element_info();
    $contextual_links['contextual_links']['#contextual_links'] = array(
            'designssquare_com_theme_blocks' => array('designssquare-blocks', array('ds_about')),
            );


    global $user;
    return array(
        'block_api' => array(
            'title' => $title,
            'content' => $content,
            'image' => $img_url,
            'ref_page' => $reference_page,
            'contextual_links' => ($user->uid == 0) ? '' : $contextual_links['contextual_links']
        )
    );
}

/******BLOCK - Social/Elsewhere ********/
function _social_block_form()
{
    return array(
        'ds_social__content_block__title' => array(
            '#type' => 'textfield',
            '#prefix' => t('Title'),
            '#default_value' => variable_get('ds_social__content_block__title', ''),
        ),
        'ds_social__content_block__content' => array(
            '#type' => 'textarea',
            '#rows' => '5',
            '#prefix' => t('Content'),
//            '#title' => t('Enter content for the block'),
            '#default_value' => variable_get('ds_social__content_block__content', ''),
        ),

    );

}

function _social_block_form_process($form_state){
    if($form_state['form_id'] == 'block_admin_configure'){
        variable_set('ds_social__content_block__title', $form_state['ds_social__content_block__title']);
        variable_set('ds_social__content_block__content', $form_state['ds_social__content_block__content']);
    }else{
        variable_set('ds_social__content_block__title', $form_state['values']['ds_social__content_block__title']);
        variable_set('ds_social__content_block__content', $form_state['values']['ds_social__content_block__content']);
    }
}

function _social_block_feed()
{

    $title = variable_get('ds_social__content_block__title', '');
    $content = variable_get('ds_social__content_block__content', '');

    module_load_include('module', 'contextual');
    $contextual_links = contextual_element_info();
    $contextual_links['contextual_links']['#contextual_links'] = array(
        'designssquare_com_theme_blocks' => array('designssquare-blocks', array('ds_social')),
    );


    global $user;
    return array(
        'block_api' => array(
            'title' => $title,
            'content' => $content,
            'contextual_links' => ($user->uid == 0) ? '' : $contextual_links['contextual_links']
        )
    );
}

/******BLOCK - Copyrights ******/
function _copy_rights_block_form()
{
    return array(
        'ds_copy_rights__content_block__content' => array(
            '#type' => 'textarea',
            '#rows' => '5',
            '#prefix' => t('Content'),
            '#default_value' => variable_get('ds_copy_rights__content_block__content', ''),
        ),

    );

}

function _copy_rights_block_form_process($form_state){
    if($form_state['form_id'] == 'block_admin_configure'){
        variable_set('ds_copy_rights__content_block__content', $form_state['ds_copy_rights__content_block__content']);
    }else{
        variable_set('ds_copy_rights__content_block__content', $form_state['values']['ds_copy_rights__content_block__content']);
    }
}

function _copy_rights_block_feed()
{

    $content = variable_get('ds_copy_rights__content_block__content', '');

    module_load_include('module', 'contextual');
    $contextual_links = contextual_element_info();
    $contextual_links['contextual_links']['#contextual_links'] = array(
        'designssquare_com_theme_blocks' => array('designssquare-blocks', array('ds_copy_rights')),
    );


    global $user;
    return array(
        'block_api' => array(
            'content' => $content,
            'contextual_links' => ($user->uid == 0) ? '' : $contextual_links['contextual_links']
        )
    );
}

/*******BLOCK - Announcement******/
function _announcement_block_form()
{
    return array(
        'ds_announcement__content_block__content' => array(
            '#type' => 'textarea',
            '#rows' => '5',
            '#prefix' => t('Content'),
            '#default_value' => variable_get('ds_announcement__content_block__content', ''),
        ),

    );

}

function _announcement_block_form_process($form_state){
    if($form_state['form_id'] == 'block_admin_configure'){
        variable_set('ds_announcement__content_block__content', $form_state['ds_announcement__content_block__content']);
    }else{
        variable_set('ds_announcement__content_block__content', $form_state['values']['ds_announcement__content_block__content']);
    }
}

function _announcement_block_feed()
{

    $content = variable_get('ds_announcement__content_block__content', '');

    module_load_include('module', 'contextual');
    $contextual_links = contextual_element_info();
    $contextual_links['contextual_links']['#contextual_links'] = array(
        'designssquare_com_theme_blocks' => array('designssquare-blocks', array('ds_announcement')),
    );


    global $user;
    return array(
        'block_api' => array(
            'content' => $content,
            'contextual_links' => ($user->uid == 0) ? '' : $contextual_links['contextual_links']
        )
    );
}

/******BLOCK - Recent Posts**********/
function _recent_tweets_block_form()
{
    return array(
        'ds_recent_tweets__content_block__title' => array(
            '#type' => 'textfield',
            '#prefix' => t('Title'),
            '#default_value' => variable_get('ds_recent_tweets__content_block__title', ''),
        ),
        'ds_recent_tweets__content_block__link' => array(
            '#type' => 'textfield',
            '#prefix' => t('Link To Twitter'),
            '#suffix' => t('(i.e. twitter.com/company)'),
            '#default_value' => variable_get('ds_about__content_block__link', ''),
        ),
        'ds_recent_tweets__content_block__button_text' => array(
            '#type' => 'textfield',
            '#prefix' => t('Button Text'),
            '#default_value' => variable_get('ds_about__content_block__button_text', ''),
        ),
        'ds_recent_tweets__content_block__content' => array(
            '#type' => 'textarea',
            '#rows' => '5',
            '#prefix' => t('Content'),
            '#default_value' => variable_get('ds_recent_tweets__content_block__content', ''),
        ),

    );

}

function _recent_tweets_block_form_process($form_state){
    if($form_state['form_id'] == 'block_admin_configure'){
        variable_set('ds_recent_tweets__content_block__title', $form_state['ds_recent_tweets__content_block__title']);
        variable_set('ds_recent_tweets__content_block__link', $form_state['ds_recent_tweets__content_block__link']);
        variable_set('ds_recent_tweets__content_block__button_text', $form_state['ds_recent_tweets__content_block__button_text']);
        variable_set('ds_recent_tweets__content_block__content', $form_state['ds_recent_tweets__content_block__content']);
    }else{
        variable_set('ds_recent_tweets__content_block__title', $form_state['values']['ds_recent_tweets__content_block__title']);
        variable_set('ds_recent_tweets__content_block__link', $form_state['values']['ds_recent_tweets__content_block__link']);
        variable_set('ds_recent_tweets__content_block__button_text', $form_state['values']['ds_recent_tweets__content_block__button_text']);
        variable_set('ds_recent_tweets__content_block__content', $form_state['values']['ds_recent_tweets__content_block__content']);
    }
}

function _recent_tweets_block_feed()
{
    $title = variable_get('ds_recent_tweets__content_block__title', '');
    $link_to_twitter = '/'.variable_get('ds_recent_tweets__content_block__link', '#');
    $button_text = variable_get('ds_recent_tweets__content_block__button_text', '');
    $content = variable_get('ds_recent_tweets__content_block__content', '');

    module_load_include('module', 'contextual');
    $contextual_links = contextual_element_info();
    $contextual_links['contextual_links']['#contextual_links'] = array(
        'designssquare_com_theme_blocks' => array('designssquare-blocks', array('ds_recent_tweets')),
    );


    global $user;
    return array(
        'block_api' => array(
            'title' => $title,
            'content' => $content,
            'button_text' => $button_text,
            'link_to_twitter' => $link_to_twitter,
            'contextual_links' => ($user->uid == 0) ? '' : $contextual_links['contextual_links']
        )
    );
}