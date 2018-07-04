package main

import (
    //"fmt"
    "math/rand"
    "time"
    "net/smtp"
    "bytes"
    "html/template"
    "log"
    //"os"
    "strconv"
)

type EmailUser struct {
    Username    string
    Password    string
    EmailServer string
    Port        int
}

const AVSENDER = "alcarlsen@gmail.com"
var emailUser = EmailUser{"alcarlsen","dnbfvtetkfbxmqyx","smtp.gmail.com",587}
var auth = smtp.PlainAuth("", emailUser.Username, emailUser.Password, emailUser.EmailServer)

type EpostMal struct {
    EpostAvsender string
    EpostMottaker string
    GaveAvsender  string
    GaveMottaker  string
}

const emailTemplate = `From: {{.EpostAvsender}};
To: {{.EpostMottaker}};
Subject: Strengt fortrolig: Nisseplaner

Hei {{.GaveAvsender}}

Dere skal gi gaver til: {{.GaveMottaker}}

Gledelig hilsen,
Adm.Nisse
`
func Shuffle(a []Couple) {
    for i := range a {
        j := rand.Intn(i + 1)
        a[i], a[j] = a[j], a[i]
    }
}

type Couple struct{
     name string
     email1 string
     email2 string
}

func (c *Couple) santafy(recipient *Couple){

var err error
var doc bytes.Buffer

        context := &EpostMal{
 	  AVSENDER,
   	  c.email1 + ", " + c.email2,
	  c.name,
  	  recipient.name,
	}
	
	t := template.New("emailTemplate")
	t, err = t.Parse(emailTemplate)
	if err != nil {
    	  log.Print("error trying to parse mail template")
    	}
	err = t.Execute(&doc, context)
	if err != nil {
    	  log.Print("error trying to execute mail template")
	}

	err = smtp.SendMail(emailUser.EmailServer+":"+strconv.Itoa(emailUser.Port),
                            auth,
			    emailUser.Username,
			    []string{c.email1, c.email2},
			    doc.Bytes())

        if err != nil {
          log.Print("ERROR: attempting to send a mail ", err)
        }

	//doc.WriteTo(os.Stdout)
	//fmt.Println("")
}

var couples = []Couple{
  {name:"Ellen og Andreas", email1:"ellen.kristoffersen@gmail.com",  email2:"alcarlsen@gmail.com"},
  {name:"Torunn og Erlend", email1:"torunn.kristoffersen@gmail.com", email2:"erlend.p.s.sande@gmail.com"},
  {name:"Astrid og Pierre", email1:"astridkristoffersen@gmail.com",  email2:"pierrebeauguitte@gmail.com"},
  {name:"Dag og Unni",      email1:"unni.kristoffersen@bfk.no",      email2:"dag.kristoffersen53@gmail.com"},
}

func main() {

        rand.Seed(time.Now().UnixNano())
	Shuffle(couples)

	var mIndex = rand.Intn(1000) % len(couples)
	for mIndex == 0 {
	  mIndex = rand.Intn(1000) % len(couples)
	}

	for key, couple := range couples{
          couple.santafy(&couples[(key + mIndex) % len(couples)])          
        }
}
