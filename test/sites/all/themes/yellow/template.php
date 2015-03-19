<?php

/**
 * @file
 * template.php
 */

function _custom_paragraphs_displays($content) {

   // dpm($content);
    if (isset($content['field_content_paragraph'])) {

        $i = 0;
        $background_colors = '';
        echo '<div class="paragraph-items">';
        while (isset($content['field_content_paragraph'][$i]['entity'])) {
            $j = key($content['field_content_paragraph'][$i]['entity']['paragraphs_item']);
            switch ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['#bundle']):

                case 'half_image_left': //1/2-1/2 with image left (half_image_left)

                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    @$image_bleed = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image_bleed']['#items'][0]['value'];

                    @$butn_link = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "class='btn'" : 'class="arrow-link"';

                    echo '<div class="half_image_left ' . @$background_colors . '">';
                    echo '<div class="container">';
                    echo '<div class="left col-lg-6 col-md-6 col-sm-6 col-xs-12 pull-left pull-height ' . @$image_bleed . '">';
                    echo "<div class='field-type-image'>";
                    echo theme("image_style", array('style_name' => 'para_left', 'path' => $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image']['#items'][0]['uri'], 'attributes' => array('class' => 'avatar')));
                    
                    echo '</div></div><div class="right col-lg-6 col-md-6 col-sm-6 col-xs-12 ">';

                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title'])) {
                        echo '<h2>' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value'] . '</h2>';
                    }
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);
                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'])) {
                        if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'])) {
                            $link = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'];
                        } else {
                            $link = '#';
                        }
                        echo '<div class="left-arrow-container"><a href="' . $link . '" ' . @$butn_link . '>' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'] . '</a></div>';
                    }

                    echo '</div>';
                    echo '</div>';
                    echo '</div>';
                    @$background_colors = '';
                    break;

                case 'half_image_right': //1/2-1/2 with image right (half_image_right)

                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];

                    @$image_bleed = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image_bleed']['#items'][0]['value'];

                    @$butn_link = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "class='btn'" : 'class="arrow-link"';


                    echo '<div class="half_image_right ' . @$background_colors . '">';
                    echo '<div class="container">';
                    echo '<div class="left col-lg-6 col-md-6 col-sm-6 col-xs-12">';
                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title'])) {
                        echo '<h2>' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value'] . '</h2>';
                    }
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);
                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'])) {
                        if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'])) {
                            $link = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'];
                        } else {
                            $link = '#';
                        }
                        echo '<div class="left-arrow-container"><a href="' . $link . '" ' . @$butn_link . '>' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'] . '</a></div>';
                    }


                    echo '</div>';
                    echo '<div class="right col-lg-6 col-md-6 col-sm-6 col-xs-12 pull-right pull-height ' . @$image_bleed . '">';
                    echo theme("image_style", array('style_name' => 'para_left', 'path' => $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image']['#items'][0]['uri'], 'attributes' => array('class' => 'avatar')));
                    echo '</div>';
                    echo '</div>';
                    echo '</div>';
                    $background_colors = '';

                    break;

                case 'text_image': // Full width text with optional image (text_image)


                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_full_background']['#items'][0]['value'])) {
//                        $fullwidth = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_full_background']['#items'][0]['value']) ? "full-image" : " ";
                    }
                    echo '<div class="text_image ' . @$background_colors . '">';
                    echo '<div class="container full-image">';
                    echo "<h2>" . render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value']) . "</h2>";
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);
                    echo '</div>';
                    echo '</div>';
                    @$background_colors = '';


                    break;

                case "text_with_background_image": //full image (text_with_background_image)

                    echo '<div class="text_with_background_image">';

                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image'])) {
                        echo '<div class="back-image">';
                        echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image']);
                        echo '</div>';
                    }
                    echo '<div class="container">';
                    if (isset($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body'])) {

                        echo '<div class="description col-lg-4 col-md-6 col-sm-5 col-xs-12">';
                        echo "<h2>" . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value'] . "</h2>";
                        echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']);
                        echo '</div>';
                    }
                    echo '</div>';
                    echo '</div>';


                    break;

                case "text_with_description": //2\2 text with 2/2 description (text_with_description)

                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    @$butn_link1 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "btn" : 'arrow-link';
                    @$butn_link2 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button2']['#items'][0]['value']) ? "btn" : 'arrow-link';

                    echo '<div class="text_with_description ' . @$background_colors . '">';
                    echo '<div class="container">';
                    echo '<h2 class="title">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value']);
                    echo '</h2>';
                    echo '<div class="first-col col-lg-6 col-md-6 col-sm-6 col-xs-12">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link1 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['title'] . '</a>';
                    }
                    echo '</div>';
                    echo '<div class="second-col col-lg-6 col-md-6 col-sm-6 col-xs-12">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_addtional_info']['#items'][0]['value']);
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link2 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['title'] . '</a>';
                    }

                    echo '</div>';
                    echo '</div>';
                    echo '</div>';
                    @$background_colors = '';
                    break;
                case "three_images_text": //1/3-1/3-1/3 with text and/or images (three_images_text)


                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    @$butn_link1 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "btn" : 'arrow-link';
                    @$butn_link2 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button2']['#items'][0]['value']) ? "btn" : 'arrow-link';
                    @$butn_link3 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button3']['#items'][0]['value']) ? "btn" : 'arrow-link';


                    echo '<div class="three_images_text ' . @$background_colors . '">';
                    echo '<div class="container">';
                    echo '<div class="first-col col-lg-4 col-md-4 col-sm-4 col-xs-12">';
                    echo '<div class="three-col-img">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_logo1']);
                    echo '</div>';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);

                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link1 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'] . '</a>';
                    }

                    echo '</div>';
                    echo '<div class="second-col col-lg-4 col-md-4 col-sm-4 col-xs-12">';
                    echo '<div class="three-col-img">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_logo2']);
                    echo '</div>';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_addtional_info']['#items'][0]['value']);
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link2 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link2']['#items'][0]['title'] . '</a>';
                    }
                    echo '</div>';

                    echo '<div class="third-col col-lg-4 col-md-4 col-sm-4 col-xs-12">';
                    echo '<div class="three-col-img">';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_logo3']);
                    echo '</div>';
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_description3']['#items'][0]['value']);
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link3 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link3']['#items'][0]['title'] . '</a>';
                    }
                    echo '</div>';
                    echo '</div>';
                    echo '</div>';
                    @$background_colors = '';

                    break;


                case "slider_images_with_description_": //Slider images with description (slider_images_with_description_)

                    echo '<div class="slider_images_with_description_ remind">';
                    echo '<section class="slider">';
                    echo '<div class="flexslider">';
                    echo '<ul class="slides">';

                    $imagedata = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_case_image']['#items'];
                    foreach ($imagedata as $image1) {
                        // dpm($image1);
                        echo "<li>" . render($image1['slide']) . '<div class="slider-descp"><div class="container"><div class="slider-desc-txt">' . render($image1['item']['image_field_caption']['value']) . "</div></div></div></li>";
                        // echo "<li>" . render($image1['slide']) .'<div class="slider-descp">'. render($image1['item']['image_field_caption']['value'])."</div></li>";
                    }

                    echo '</ul>';
                    echo '</div>';
                    echo '</section>';
                    echo '</div>';

                    break;

                case "teaser":
                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    @$butn_link = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "btn" : 'arrow-link';
                    echo '<div class="organistion-full ' . @$background_colors . '">';
                    echo '<div class="container">';
                    echo '<div class="organistion">';

                    if ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value']) {
                        echo '<h2>' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value'] . '</h2>';
                    }

                    echo '<div class="button"><a class="' . @$butn_link . ' primary-btn btn-default" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'] . '</a></div>';
                    echo '</div>
</div></div>';
                    break;

                case "full_width_small":
                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    @$butn_link2 = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_link_is_button']['#items'][0]['value']) ? "btn" : 'arrow-link';
                    echo '<div class="full_width_small ' . @$background_colors . '">';
                    echo '<div class="container full-text-top text-center">';
                    echo "<h2>" . render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_half_left_title']['#items'][0]['value']) . "</h2>";
                    echo render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_body']['#items'][0]['value']);
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'])) {
                        echo '<a class="' . @$butn_link2 . '" href="' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['url'] . '">' . $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_button_link']['#items'][0]['title'] . '</a>';
                    }
                    echo '</div><div class="container">';
                    echo '<div class="col-lg-12 col-md-8 col-sm-8 col-xs-10">' . render($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_image']) . "</div></div>";
                    echo '</div>';
                    // $fullwidth = ' ';
                    @$background_colors = '';
                    break;

                case "normal_paragraph":
                    @$background_colors = $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_background_color']['#items'][0]['value'];
                    if (!empty($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_full_width']['#items'][0]['value'])) {
                        $fullwidth_data = ($content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_full_width']['#items'][0]['value']) ? "full-width-data" : "container";
                    }
                    echo '<div class="normal_paragraph ' . @$background_colors . '">';
                    echo '<div class=" ' . $fullwidth_data . '">';
                    echo $content['field_content_paragraph'][$i]['entity']['paragraphs_item'][$j]['field_addtional_info']['#items'][0]['value'];
                    echo '</div>';
                    echo '</div>';
                    $fullwidth_data = 'container';
                    @$background_colors = '';
            endswitch;

            $i++;
        }
        echo "</div>";
    }
}
