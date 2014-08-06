<?php
  class Inquiry {
    public $standardHeader;
    public $account;
    public $questionsAndAnswers;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->questionsAndAnswers = null;
    }
  }
  
  class AccountHistory {
    public $standardHeader;
    public $account;
    public $report;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->report = new Report();
    }
  }
  
  class EmployeeReport {
    public $standardHeader;
    public $reportOnEmployeeId;
    public $report;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->reportOnEmployeeId = "";
      $this->report = new Report();
    }
  }

  class TerminalReport {
    public $standardHeader;
    public $report;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->report = new Report();
    }
  }
  
  class GiftIssuance {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }

  class GiftRedemption {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $includeTip;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo; 
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->includeTip = "";
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class Enrollment {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class LoyaltyIssuance {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class LoyaltyRedemption {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class PromotionIssuance {
    public $standardHeader;
    public $account;
    public $activating;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class PromotionRedemption {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->account = new Account();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class MerchandiseReturn {
    public $standardHeader;
    public $account;
    public $activating;
    public $amount;
    public $promotionCodes;
    public $questionsAndAnswers;
    public $customerInfo;  
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->amount = new Amount();
      $this->promotionCodes = null;
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class MultipleIssuance {
    public $standardHeader;
    public $multipleIssuanceComponent;
    public $amount;
    public $activating;
    public $promotionCodes;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->multipleIssuanceComponent = new MultipleIssuanceComponent();
      $this->amount = new Amount();
      $this->activating = "";
      $this->promotionCodes = null;
    }
  }
  
  class QuickTransaction {
    public $standardHeader;
    public $account;
    public $quickCode;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->quickCode = "";
    }
  }
  
  class Transfer {
    public $standardHeader;
    public $account;
    public $activating;
    public $transfer;
    public $questionsAndAnswers;
    public $customerInfo;  
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->activating = "";
      $this->transfer = new TransferComponent();
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo(); 
    }
  } 
  
  class Renew {
    public $standardHeader;
    public $account;
    public $newExpirationDate;
    public $questionsAndAnswers;
    public $customerInfo;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->newExpirationDate = "";
      $this->questionsAndAnswers = null;
      $this->customerInfo = new CustomerInfo();
    }
  }
  
  class Tip {
    public $standardHeader;
    public $account;
    public $search;
    public $amount;
    public $questionsAndAnswers;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->search = new Search();
      $this->amount = new Amount();
      $this->questionsAndAnswers = null;
    }
  }
  
  class VoidTransaction {
    public $standardHeader;
    public $account;
    public $search;
    public $questionsAndAnswers;
    
    function __construct() {
      $this->standardHeader = new StandardHeader();
      $this->account = new Account();
      $this->search = new Search();
      $this->questionsAndAnswers = null;
    }
  }
  
  class StandardHeader {
    public $requestId;
    public $localeId;
    public $systemId;
    public $clientId;
    public $locationId;
    public $terminalId;
    public $terminalDateTime;
    public $initiatorType; 
    public $initiatorId;
    public $initiatorPassword;
    public $externalId;
    public $batchId;
    public $batchReference;
    
    function __construct() {
      $this->requestId = "";
      $this->localeId = "";
      $this->systemId = "";
      $this->clientId = "";
      $this->locationId = "";
      $this->terminalId = "";
      $this->terminalDateTime = "";
      $this->initiatorType = ""; 
      $this->initiatorId = "";
      $this->initiatorPassword = "";
      $this->externalId = "";
      $this->batchId = "";
      $this->batchReference = "";
    }
  }
  
  class Account {  
    public $accountId;
    public $pin;
    public $entryType;
    
    function __construct() {
      $this->accountId = "";
      $this->pin = "";
      $this->entryType = "";
    }
  }
  
  class CustomerInfo {
    public $customerType;
    public $firstName;
    public $middleName;
    public $lastName;
    public $address1;
    public $address2;
    public $city;
    public $state;
    public $postal;
    public $country;
    public $mailPref;
    public $phone;
    public $isMobile;
    public $phonePref;
    public $email;
    public $emailPref;
    public $birthday;  
    public $anniversary;
    public $gender;
    
    function __construct() {
      $this->customerType = "";
      $this->firstName = "";
      $this->middleName = "";
      $this->lastName = "";
      $this->address1 = "";
      $this->address2 = "";
      $this->city = "";
      $this->state = "";
      $this->postal = "";
      $this->country = "";
      $this->mailPref = "";
      $this->phone = "";
      $this->isMobile = "";
      $this->phonePref = "";
      $this->email = "";
      $this->emailPref = "";
      $this->birthday = "";  
      $this->anniversary = "";
      $this->gender = "";
    }
  } 
   
  class MultipleIssuanceComponent {
    public $specifiedBy;
    public $accounts;   
    
    function __construct() {
      $this->specifiedBy = "";
      $this->accounts = null;   
    }
  }
  
  class Report {
    public $type;
    public $minimumDate;
    public $maximumDate;
    public $offset;
    public $maxRecords;
    
    function __construct() {
      $this->type = "";
      $this->minimumDate = "";
      $this->maximumDate = "";
      $this->offset = "";
      $this->maxRecords = "";
    }
  }
  
  class Search {  
    public $batchId;
    public $batchReference;
    public $transactionId;
    public $approvalCode;
    
    function __construct() {
      $this->batchId = "";
      $this->batchReference = "";
      $this->transactionId = "";
      $this->approvalCode = "";
    }
  }  
  
  class TransferComponent {
    public $destAccountId;
    public $destPin;
    public $destEntryType;
    public $closeReason;
    public $valueCode;
    public $enteredAmount;
    
    function __construct() {
      $this->destAccountId = "";
      $this->destPin = "";
      $this->destEntryType = "";
      $this->closeReason = "";
      $this->valueCode = "";
      $this->enteredAmount = "";
    }
  }
  
  class Amount { 
    public $valueCode;
    public $enteredAmount;
    public $nsfAllowed;
    
    function __construct() {
      $this->valueCode = "";
      $this->enteredAmount = "";
      $this->nsfAllowed = "";
    }
  }
    
  class MultipleIssuanceAccount {
    public $accountId;
    public $pin;
    public $entryType ;
    
    function __construct() {
      $this->accountId = "";
      $this->pin = "";
      $this->entryType = "";
    }
  }
  
  class PromotionCodes {
    public $code;
    public $quantity;
    
    function __construct() {
      $this->code = "";
      $this->quantity = "";
    }
  }
  
  class QuestionAndAnswer {
    public $code;
    public $answer;
    
    function __construct() {
      $this->code = "";
      $this->answer = "";
    }
  }
?>