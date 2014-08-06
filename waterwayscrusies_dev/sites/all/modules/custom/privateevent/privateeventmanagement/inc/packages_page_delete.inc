<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function private_events_packages_delete_page($packageId){
   
    $deletepdf=new PrivateEventPackages();
    $getpdfdirectory=$deletepdf->getPrivateEventPackageDataByPackageId($packageId);
    if($getpdfdirectory!=false){
        $deletdownloadpdf=file_load($getpdfdirectory->downloadPdfFid);
        $deletecateringpdf=file_load($getpdfdirectory->cateringPdfFid);
        drupal_unlink($deletdownloadpdf->uri);
        drupal_unlink($deletecateringpdf->uri);
        $deletepdf->deletePackagespage($packageId);
   }
    $path="private-events/packages/".$_GET['eventId']."/list";
    drupal_goto($path);
}