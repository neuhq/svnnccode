#include <stdio.h>
#include <memory.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>


class ParseFSM
{
public:
   /** SAX2 callback when an element start has been detected by the parser. It provides the namespace informations for the element, as well as the new namespace declarations on the element.
      ctx:  the user data (XML parser context)
      localname:  the local name of the element
      prefix:  the element namespace prefix if available
      URI:  the element namespace name if available
      nb_namespaces: number of namespace definitions on that node
      namespaces: pointer to the array of prefix/URI pairs namespace definitions
      nb_attributes: the number of attributes on that node
      nb_defaulted:  the number of defaulted attributes. The defaulted ones are at the end of the array
      attributes: pointer to the array of (localname/prefix/URI/value/end) attribute values.
      */
	static void charactersSAXFunc(void* ctx, const xmlChar *ch, int len)
	{
		printf("%s\n", ch);
	}
   static void startElementNs( void * ctx, 
                               const xmlChar * localname, 
                               const xmlChar * prefix, 
                               const xmlChar * URI, 
                               int nb_namespaces, 
                               const xmlChar ** namespaces, 
                               int nb_attributes, 
                               int nb_defaulted, 
                               const xmlChar ** attributes )
   {
      ParseFSM &fsm = *( static_cast<ParseFSM *>( ctx ) );
      printf( "startElementNs: name = '%s' prefix = '%s' uri = (%p)'%s'\n", localname, prefix, URI, URI );
      for ( int indexNamespace = 0; indexNamespace < nb_namespaces; ++indexNamespace )
      {
         const xmlChar *prefix = namespaces[indexNamespace*2];
         const xmlChar *nsURI = namespaces[indexNamespace*2+1];
         printf( "  namespace: name='%s' uri=(%p)'%s'\n", prefix, nsURI, nsURI );
      }

      unsigned int index = 0;
      for ( int indexAttribute = 0; 
            indexAttribute < nb_attributes; 
            ++indexAttribute, index += 5 )
      {
         const xmlChar *localname = attributes[index];
         const xmlChar *prefix = attributes[index+1];
         const xmlChar *nsURI = attributes[index+2];
         const xmlChar *valueBegin = attributes[index+3];
         const xmlChar *valueEnd = attributes[index+4];
         std::string value( (const char *)valueBegin, (const char *)valueEnd );
         printf( "  %sattribute: localname='%s', prefix='%s', uri=(%p)'%s', value='%s'\n",
                 indexAttribute >= (nb_attributes - nb_defaulted) ? "defaulted " : "",
                 localname,
                 prefix,
                 nsURI,
                 nsURI,
                 value.c_str() );
      }
   }

   /** SAX2 callback when an element end has been detected by the parser. It provides the namespace informations for the element.
      ctx:  the user data (XML parser context)
      localname:  the local name of the element
      prefix:  the element namespace prefix if available
      URI:  the element namespace name if available
      */
   static void endElementNs( void * ctx, 
                             const xmlChar * localname, 
                             const xmlChar * prefix, 
                             const xmlChar * URI )
   {
      ParseFSM &fsm = *( static_cast<ParseFSM *>( ctx ) );
      printf( "endElementNs: name = '%s' prefix = '%s' uri = '%s'\n", localname, prefix, URI );
   }

   /** Display and format an error messages, callback.
      ctx:  an XML parser context
      msg:  the message to display/transmit
      ...:  extra parameters for the message display
      */
   static void error( void * ctx, 
                      const char * msg, 
                      ... )
   {
      ParseFSM &fsm = *( static_cast<ParseFSM *>( ctx ) );
      va_list args;
      va_start(args, msg);
      vprintf( msg, args );
      va_end(args);
   }

   /** Display and format a warning messages, callback.
      ctx:  an XML parser context
      msg:  the message to display/transmit
      ...:  extra parameters for the message display
      */
   static void warning( void * ctx, 
                        const char * msg, 
                        ... )
   {
      ParseFSM &fsm = *( static_cast<ParseFSM *>( ctx ) );
      va_list args;
      va_start(args, msg);
      vprintf( msg, args );
      va_end(args);
   }
};


int main(int argc, const char * argv[])
{
   std::string xmlIn = 
      "<test:Plan xmlns:test='http://test.org/schema'>"
      "<test:Case name='test1' emptyAttribute='' test:ns_id='auio'>"
      "</test:Case>"
      "</test:Plan>";

   /*
    * this initialize the library and check potential ABI mismatches
    * between the version it was compiled for and the actual shared
    * library used.
    */
   LIBXML_TEST_VERSION

   xmlSAXHandler saxHandler; // See http://xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
   memset( &saxHandler, 0, sizeof(saxHandler) );
   // Using xmlSAXVersion( &saxHandler, 2 ) generate crash as it sets plenty of other pointers...
   saxHandler.initialized = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
   saxHandler.startElementNs = &ParseFSM::startElementNs;
   saxHandler.endElementNs = &ParseFSM::endElementNs;
   saxHandler.warning = &ParseFSM::warning;
   saxHandler.error = &ParseFSM::error;
   saxHandler.characters = &ParseFSM::charactersSAXFunc;

   ParseFSM fsm;
//   int result = xmlSAXUserParseMemory( &saxHandler, &fsm, xmlIn.c_str(), int(xmlIn.length()) );
   int result = xmlSAXUserParseFile(&saxHandler, &fsm, "/mnt/f/zhwiki-latest-pages-articles.xml");
   if ( result != 0 )
   {
      printf("Failed to parse document.\n" );
      return 1;
   }

   /*
    * Cleanup function for the XML library.
    */
   xmlCleanupParser();
   /*
    * this is to debug memory for regression tests
    */
   xmlMemoryDump();

   return 0;
}

