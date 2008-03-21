// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit tests for the Element and Field classes.

#include "kml/dom/element.h"
#include "kml/util/unit_test.h"
#include "kml/dom/kml_factory.h"

namespace kmldom {

// This tests the Element class.
class ElementTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ElementTest);
  CPPUNIT_TEST(TestUnknowns);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    element_ = new TestElement();
  }
  void tearDown() {
    delete element_;
  }

 protected:
  void TestUnknowns();

 private:
  // Default constructor is protected.
  class TestElement : public Element {
  };
  TestElement* element_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ElementTest);

void ElementTest::TestUnknowns() {
  // Unrecognised elements:
  const std::string unknown1("unknownFoo");
  const std::string unknown2("unknownBar");
  element_->AddUnknownElement(unknown1);
  element_->AddUnknownElement(unknown2);
  const std::vector<std::string>& unknown_elements_array =
      element_->unknown_elements_array();
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), unknown_elements_array.size());
  CPPUNIT_ASSERT_EQUAL(unknown1, unknown_elements_array[0]);
  CPPUNIT_ASSERT_EQUAL(unknown2, unknown_elements_array[1]);

  // Recognized but misplaced elements:
  Element* legal_name = KmlFactory::GetFactory()->CreateFieldById(Type_name);
  Element* legal_open = KmlFactory::GetFactory()->CreateFieldById(Type_open);
  element_->AddElement(legal_name);
  element_->AddElement(legal_open);
  const std::vector<Element*>& unknown_legal_elements_array =
      element_->unknown_legal_elements_array();
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2),
                       unknown_legal_elements_array.size());
  CPPUNIT_ASSERT_EQUAL(Type_name, unknown_legal_elements_array[0]->Type());
  CPPUNIT_ASSERT_EQUAL(Type_open, unknown_legal_elements_array[1]->Type());
}

// This tests the Field class.
class FieldTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FieldTest);
  CPPUNIT_TEST(TestSetBool);
  CPPUNIT_TEST(TestSetDouble);
  CPPUNIT_TEST(TestSetInt);
  CPPUNIT_TEST(TestSetEnum);
  CPPUNIT_TEST(TestSetString);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestSetBool();
  void TestSetDouble();
  void TestSetInt();
  void TestSetEnum();
  void TestSetString();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldTest);

// This tests Field's SetBool() method.
void FieldTest::TestSetBool() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Field* field = factory->CreateFieldById(Type_open);
  // Pathological, but well defined case.  Note: SetBool always deletes field.
  CPPUNIT_ASSERT_EQUAL(false, field->SetBool(NULL));

  // Handle the 5 variants of bool: "1", "true", "0", "false", garbage
  bool open;

  // <open>1</open>
  // Parser finds "<open>" and maps to Type_open which is simple and thus
  // it creates a Field.
  field = factory->CreateFieldById(Type_open);
  // Parser gathers character data.
  field->set_char_data("1");
  // Parse passes Field to parent element which accepts and converts
  // and deletes the Field.
  open = false;  // Verify that SetBool changes to true.
  CPPUNIT_ASSERT(field->SetBool(&open));  // SetBool deletes field.
  CPPUNIT_ASSERT(open);

  // <open>0</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("0");
  open = true;
  CPPUNIT_ASSERT(field->SetBool(&open));  // SetBool deletes field.
  CPPUNIT_ASSERT(false == open);

  // <open>true</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("true");
  open = false;
  CPPUNIT_ASSERT(field->SetBool(&open));  // SetBool deletes field.
  CPPUNIT_ASSERT(open);

  // <open>false</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("false");
  open = true;
  CPPUNIT_ASSERT(field->SetBool(&open));  // SetBool deletes field.
  CPPUNIT_ASSERT(false == open);

  // <open>garbage</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("garbage");
  open = true;
  CPPUNIT_ASSERT(field->SetBool(&open));  // SetBool deletes field.
  CPPUNIT_ASSERT(false == open);
}

// This tests Field's SetDouble() method.
void FieldTest::TestSetDouble() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Field* field = factory->CreateFieldById(Type_north);
  // Pathological, but well defined case.  Note: SetDouble always deletes field.
  CPPUNIT_ASSERT_EQUAL(false, field->SetDouble(NULL));

  // <north>37.123</north>
  field = factory->CreateFieldById(Type_north);
  field->set_char_data("37.123");
  double north = 1.1;
  CPPUNIT_ASSERT(field->SetDouble(&north));  // SetDouble() deletes field.
  CPPUNIT_ASSERT_EQUAL(37.123, north);
}

// This tests Field's SetInt() method.
void FieldTest::TestSetInt() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Field* field = factory->CreateFieldById(Type_drawOrder);
  // Pathological, but well defined case.  Note: SetInt always deletes field.
  CPPUNIT_ASSERT_EQUAL(false, field->SetInt(NULL));

  // <drawOrder>10</drawOrder>
  field = factory->CreateFieldById(Type_drawOrder);
  field->set_char_data("10");
  int draworder = 11;  // not 10
  CPPUNIT_ASSERT(field->SetInt(&draworder));  // SetInt() deletes field.
  CPPUNIT_ASSERT_EQUAL(10, draworder);
}

// This tests Field's SetEnum() method.
void FieldTest::TestSetEnum() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Field* field = factory->CreateFieldById(Type_altitudeMode);
  // Pathological, but well defined case: null pointer to enum val.
  // Note: SetEnum always deletes field.
  CPPUNIT_ASSERT_EQUAL(false, field->SetEnum(NULL));

  int altitudemode;

  // A normal case:
  // <altitudeMode>relativeToGround</altitudeMode>
  field = factory->CreateFieldById(Type_altitudeMode);
  field->set_char_data("relativeToGround");
  altitudemode = kmldom::ALTITUDEMODE_ABSOLUTE;  // Not relativeToGround.
  CPPUNIT_ASSERT(field->SetEnum(&altitudemode));
  // Note: SetEnum() deletes field.
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_RELATIVETOGROUND),
                       altitudemode);

  // Unknown enum val behaves as is that enum did not exist:
  // <altitudeMode>reach-for-the-stars</altitudeMode>
  field = factory->CreateFieldById(Type_altitudeMode);
  field->set_char_data("reach-for-the-stars");
  altitudemode = kmldom::ALTITUDEMODE_ABSOLUTE;
  CPPUNIT_ASSERT_EQUAL(false, field->SetEnum(&altitudemode));
  // Note: SetEnum() deletes field.
  // altitudemode remains unchanged:
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
                       altitudemode);

  // Pathologiccal case: Field is not enum.  The addressed val is not
  // touched and false is returned.
  field = factory->CreateFieldById(Type_visibility);
  int not_touched = 42;
  CPPUNIT_ASSERT_EQUAL(false, field->SetEnum(&not_touched));
  CPPUNIT_ASSERT_EQUAL(42, not_touched);
}

// This tests Field's SetString() method.
void FieldTest::TestSetString() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Field* field = factory->CreateFieldById(Type_description);
  // Pathological, but well defined case: null pointer to string val.
  // Note: SetString always deletes field.
  CPPUNIT_ASSERT_EQUAL(false, field->SetString(NULL));

  std::string name;

  // <name>my name</name>
  field = factory->CreateFieldById(Type_name);
  const char* kMyName = "my name";
  field->set_char_data(kMyName);
  CPPUNIT_ASSERT_EQUAL(true, field->SetString(&name));
  CPPUNIT_ASSERT_EQUAL(std::string(kMyName), name);
}

}  // end namespace kmldom

TEST_MAIN