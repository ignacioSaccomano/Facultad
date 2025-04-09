/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "defines.h"
#include "kassert.h"

static pd_entry_t* kpd = (pd_entry_t*)KERNEL_PAGE_DIR;
static pt_entry_t* kpt = (pt_entry_t*)KERNEL_PAGE_TABLE_0;

static const uint32_t identity_mapping_end = 0x003FFFFF;
static const uint32_t user_memory_pool_end = 0x02FFFFFF;

static paddr_t next_free_kernel_page = 0x00100000;
static paddr_t next_free_user_page = 0x00400000;

/**
 * kmemset asigna el valor c a un rango de memoria interpretado
 * como un rango de bytes de largo n que comienza en s
 * @param s es el puntero al comienzo del rango de memoria
 * @param c es el valor a asignar en cada byte de s[0..n-1]
 * @param n es el tamaño en bytes a asignar
 * @return devuelve el puntero al rango modificado (alias de s)
*/
static inline void* kmemset(void* s, int c, size_t n) {
  uint8_t* dst = (uint8_t*)s;
  for (size_t i = 0; i < n; i++) {
    dst[i] = c;
  }
  return dst;
}

/**
 * zero_page limpia el contenido de una página que comienza en addr
 * @param addr es la dirección del comienzo de la página a limpiar
*/
static inline void zero_page(paddr_t addr) {
  kmemset((void*)addr, 0x00, PAGE_SIZE);
}


extern void mmu_init(void) {
   

  /* uint32_t temp = rcr0(); //inicializa paginacion seteando en cr0 los bits wp y pg. Mantiene los flags et y pe
   temp = temp | 0x80010000;
   lcr0(temp);
   return;
  */
}


/**
 * mmu_next_free_kernel_page devuelve la dirección física de la próxima página de kernel disponible. 
 * Las páginas se obtienen en forma incremental, siendo la primera: next_free_kernel_page
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de kernel
 */
paddr_t mmu_next_free_kernel_page(void) {
  kassert(next_free_kernel_page < identity_mapping_end,"Te pasaste, boludin(K)");
  uint32_t res = next_free_kernel_page;
  next_free_kernel_page += PAGE_SIZE;
  return (paddr_t)res;
  
}

/**
 * mmu_next_free_user_page devuelve la dirección de la próxima página de usuarix disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de usuarix
 */
paddr_t mmu_next_free_user_page(void) {
  kassert(next_free_user_page < user_memory_pool_end,"Te pasaste, boludin(U)");
  uint32_t res = next_free_user_page;
  next_free_user_page += PAGE_SIZE;
  return (paddr_t)res;
}

/**
 * mmu_init_kernel_dir inicializa las estructuras de paginación vinculadas al kernel y
 * realiza el identity mapping
 * @return devuelve la dirección de memoria de la página donde se encuentra el directorio
 * de páginas usado por el kernel
 */
extern paddr_t mmu_init_kernel_dir(void) {
  
  zero_page((paddr_t)(kpd));
  zero_page((paddr_t)(kpt));  
  uint32_t pde_attr = MMU_W|MMU_K|MMU_P;
  kpd[0].attrs = pde_attr;
  kpd[0].pt = (KERNEL_PAGE_TABLE_0>>12); //Asigno a la posicion 0 de la pd una pt con todas las paginas que nos interesann ahora
  uint32_t pte_attr = MMU_W|MMU_K|MMU_P;
  for(int i = 0; i < 1024; i++){

    kpt[i].attrs = pte_attr;
    kpt[i].page = i;

    }
    
    return KERNEL_PAGE_DIR;
    
  }
/**
 * mmu_map_page agrega las entradas necesarias a las estructuras de paginación de modo de que
 * la dirección virtual virt se traduzca en la dirección física phy con los atributos definidos en attrs
 * @param cr3 el contenido que se ha de cargar en un registro CR3 al realizar la traducción
 * @param virt la dirección virtual que se ha de traducir en phy
 * @param phy la dirección física que debe ser accedida (dirección de destino)
 * @param attrs los atributos a asignar en la entrada de la tabla de páginas
 */
void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
  uint32_t prev_cr3 = rcr3();
  uint8_t flushed = 0;
  pd_entry_t* new_pd;


 
  
  
  new_pd = (pd_entry_t*)(CR3_TO_PAGE_DIR(cr3));


 


 
  uint32_t CUR_PAGE_DIR_INDEX = VIRT_PAGE_DIR(virt);



  //Inicializo la page table si no esta aun presente
  if((new_pd[CUR_PAGE_DIR_INDEX].attrs & MMU_P) == 0){
    paddr_t new_page_table = mmu_next_free_kernel_page();
    zero_page(new_page_table);
    new_pd[CUR_PAGE_DIR_INDEX].pt = new_page_table>>12;

 }
  new_pd[CUR_PAGE_DIR_INDEX].attrs |= attrs;

  pt_entry_t* new_pt = (pt_entry_t*)(MMU_ENTRY_PADDR(new_pd[CUR_PAGE_DIR_INDEX].pt));
  uint32_t CUR_PAGE_TABLE_INDEX = VIRT_PAGE_TABLE(virt);
  if(phy != MMU_ENTRY_PADDR(new_pt[CUR_PAGE_TABLE_INDEX].page)){
    new_pt[CUR_PAGE_TABLE_INDEX].page = phy >> 12; //Me saco de encima el offset
    

    if(!flushed && (new_pt[CUR_PAGE_TABLE_INDEX].attrs & MMU_A) != 0){ //No habiamos hecho flush aun, y la page_table fue accedida, por lo que puede estar la traduccion en el cache

      tlbflush();
      flushed = 1;

    }
    new_pt[CUR_PAGE_TABLE_INDEX].attrs |= attrs;         
    
  
  


  }
  


  
  
  

  

  
}

/**
 * mmu_unmap_page elimina la entrada vinculada a la dirección virt en la tabla de páginas correspondiente
 * @param virt la dirección virtual que se ha de desvincular
 * @return la dirección física de la página desvinculada
 */

extern void tlb_flush(){
  tlbflush();

}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
  virt = virt & 0xFFFFF000; //Elimino el offset, si lo hay
  
  pd_entry_t* new_pd = (pd_entry_t*)(CR3_TO_PAGE_DIR(cr3));
  uint32_t CUR_PAGE_DIR_INDEX = VIRT_PAGE_DIR(virt);
  pt_entry_t* new_pt = (pt_entry_t*)(MMU_ENTRY_PADDR(new_pd[CUR_PAGE_DIR_INDEX].pt));
  uint32_t CUR_PAGE_TABLE_INDEX = VIRT_PAGE_TABLE(virt);
  
  
  


  paddr_t unlinked_page_pdir =  MMU_ENTRY_PADDR(new_pt[CUR_PAGE_TABLE_INDEX].page);
  

  new_pt[CUR_PAGE_TABLE_INDEX].page = 0;
  new_pt[CUR_PAGE_TABLE_INDEX].attrs = 0;

  tlbflush();
  
  return unlinked_page_pdir;

}

#define DST_VIRT_PAGE 0xA00000
#define SRC_VIRT_PAGE 0xB00000

/**
 * copy_page copia el contenido de la página física localizada en la dirección src_addr a la página física ubicada en dst_addr
 * @param dst_addr la dirección a cuya página queremos copiar el contenido
 * @param src_addr la dirección de la página cuyo contenido queremos copiar
 *
 * Esta función mapea ambas páginas a las direcciones SRC_VIRT_PAGE y DST_VIRT_PAGE, respectivamente, realiza
 * la copia y luego desmapea las páginas. Usar la función rcr3 definida en i386.h para obtener el cr3 actual
 */
extern void copy_page(paddr_t dst_addr, paddr_t src_addr) {
  uint32_t cr3 = rcr3();
  mmu_map_page(cr3,DST_VIRT_PAGE,dst_addr,MMU_W|MMU_K|MMU_P);//atributos: r/w, de kernel,ultimo bit 1
  mmu_map_page(cr3,SRC_VIRT_PAGE,src_addr,MMU_W|MMU_K|MMU_P);//atributos: r/W, de kernel, ultimo bit 1
  vaddr_t* src_tmp = SRC_VIRT_PAGE;
  vaddr_t* dst_tmp = DST_VIRT_PAGE;
  for(int i = 0; i < PAGE_SIZE/4; i++){

    dst_tmp[i] = src_tmp[i];
    


  }
  mmu_unmap_page(cr3,SRC_VIRT_PAGE);
  mmu_unmap_page(cr3,DST_VIRT_PAGE);
  
}

 /**
 * mmu_init_task_dir inicializa las estructuras de paginación vinculadas a una tarea cuyo código se encuentra en la dirección phy_start
 * @pararm phy_start es la dirección donde comienzan las dos páginas de código de la tarea asociada a esta llamada
 * @return el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada
 */
extern paddr_t mmu_init_task_dir(paddr_t phy_start) {
    
    paddr_t task_code1_pdir = phy_start;
    paddr_t task_code2_pdir = phy_start+PAGE_SIZE;
    paddr_t task_stack_pdir = mmu_next_free_user_page();
    paddr_t task_shared_pdir = SHARED;
    
    vaddr_t task_code1_vdir = TASK_CODE_VIRTUAL;
    vaddr_t task_code2_vdir = TASK_CODE_VIRTUAL+PAGE_SIZE;
    vaddr_t task_stack_vdir = TASK_STACK_BASE-PAGE_SIZE;
    vaddr_t task_shared_vdir = TASK_SHARED_PAGE;


    vaddr_t task_code1_attr = MMU_R|MMU_P|MMU_U; 
    vaddr_t task_code2_attr = MMU_R|MMU_P|MMU_U;
    vaddr_t task_stack_attr = MMU_W|MMU_P|MMU_U;
    vaddr_t task_shared_attr = MMU_R|MMU_P|MMU_U;
    pd_entry_t* new_cr3 = (pd_entry_t*)mmu_next_free_kernel_page();
    pd_entry_t* new_kernel_dir =(pd_entry_t*) KERNEL_PAGE_DIR;
    new_cr3[0] = new_kernel_dir[0];
    mmu_map_page(new_cr3,task_code1_vdir,task_code1_pdir,task_code1_attr);
    mmu_map_page(new_cr3,task_code2_vdir,task_code2_pdir,task_code2_attr);
    mmu_map_page(new_cr3,task_stack_vdir,task_stack_pdir,task_stack_attr);
    mmu_map_page(new_cr3,task_shared_vdir,task_shared_pdir,task_shared_attr);
    
    return new_cr3;

}

// COMPLETAR: devuelve true si se atendió el page fault y puede continuar la ejecución 
// y false si no se pudo atender
extern bool page_fault_handler(vaddr_t virt) {
  print("Atendiendo page fault...", 0, 0, C_FG_WHITE | C_BG_BLACK);
  // Chequeemos si el acceso fue dentro del area on-demand
  if(virt < ON_DEMAND_MEM_START_VIRTUAL || virt >= ON_DEMAND_MEM_END_VIRTUAL){
    print("Pedido no se encuentra en area on-demand", 0, 0, C_FG_RED | C_BG_BLACK);
    
    return false;
  }
  // En caso de que si, mapear la pagina
  else{

    uint32_t on_demand_attr = MMU_P|MMU_U|MMU_W;
    paddr_t on_demand_pdir = ON_DEMAND_MEM_START_PHYSICAL;
    uint32_t cr3 = rcr3();
    mmu_map_page(cr3,virt,on_demand_pdir,on_demand_attr);
    print("Pedido atendido                                          ", 0, 0, C_FG_GREEN | C_BG_BLACK);
    print("                                                               ",0,0,C_BG_BLACK|C_BG_BLACK);
    return true;

  }
}
